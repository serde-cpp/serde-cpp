#include <filesystem>
#include <fstream>
#include <iostream>

#include <cxxopts.hpp>
#include <cppast/libclang_parser.hpp> // for libclang_parser, libclang_compile_config, cpp_entity,...

#include "common.h"

int main(int argc, char *argv[]) {
  cxxopts::Options option_list(
      "serde_attr",
      "serde_attr - Serde attribute parser and serialization generator.\n");

  option_list.add_options()("h,help", "display this help and exit")(
      "version", "display version information and exit")(
      "v,verbose", "be verbose when parsing")(
      "fatal_errors", "abort program when a parser error occurs, instead of "
                      "doing error correction");

  option_list.add_options("compilation")("infile",
                                         "the file that are being parsed",
                                         cxxopts::value<std::string>())(
      "outfile", "the output file that will be generated",
      cxxopts::value<std::string>())(
      "database_dir",
      "set the directory where a 'compile_commands.json' file is located "
      "containing build information",
      cxxopts::value<std::string>())(
      "database_file",
      "set the file name whose configuration will be used regardless of the "
      "current file name",
      cxxopts::value<std::string>())(
      "I,include_directory", "add directory to include search path",
      cxxopts::value<std::vector<std::string>>());

  auto options = option_list.parse(argc, argv);
  if (options.count("help")) {
    std::cout << option_list.help() << std::endl;
  } else if (options.count("version")) {
    std::cout << "serde_attr version 0.1.0\n";
    std::cout
        << "Copyright (C) Natanael J. Rabello <natanaeljrabello@gmail.com>\n";
    std::cout << '\n';
    std::cout << "Using libclang version " << CPPAST_CLANG_VERSION_STRING
              << '\n';
  } else if (!options.count("infile") ||
             options["infile"].as<std::string>().empty()) {
    std::cerr << "missing infile argument" << '\n';
    return 1;
  } else if (!options.count("outfile") ||
             options["outfile"].as<std::string>().empty()) {
    std::cerr << "missing outfile argument" << '\n';
    return 1;
  } else {
    // the compile config stores compilation flags
    cppast::libclang_compile_config config;
    if (options.count("database_dir")) {
      cppast::libclang_compilation_database database(
          options["database_dir"].as<std::string>());
      if (options.count("database_file"))
        config = cppast::libclang_compile_config(
            database, options["database_file"].as<std::string>());
      // else
      // config
      //= cppast::libclang_compile_config(database,
      //options["file"].as<std::string>());
    }

    // cppast::libclang_compile_config config;
    config.fast_preprocessing(1);
    cppast::compile_flags flags;
    config.set_flags(cppast::cpp_standard::cpp_17, flags);
    if (options.count("include_directory"))
      for (auto &include :
           options["include_directory"].as<std::vector<std::string>>())
        config.add_include_dir(include);

    cppast::stderr_diagnostic_logger logger;
    if (options.count("verbose"))
      logger.set_verbose(true);

    const auto &infilename = options["infile"].as<std::string>();

    std::string outfilename = options["outfile"].as<std::string>();
    auto basepath = std::filesystem::path(outfilename).remove_filename();
    std::filesystem::create_directories(basepath);

    // pre-create output file for parsing #include of generated file
    std::ofstream outfile(outfilename);
    outfile.close();

    auto file = parse_file(config, logger, infilename,
                           options.count("fatal_errors") == 1);
    if (!file)
      return 2;

    print_ast(std::cout, *file);

    outfile.open(outfilename);
    generate_serde(outfile, *file);
  }

  return 0;
}
