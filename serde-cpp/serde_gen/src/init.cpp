#include "init.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <cxxopts.hpp>
#include <cppast/libclang_parser.hpp>

#include "common.h"

namespace serde_gen::init {

auto create_option_list() -> cxxopts::Options {
    // clang-format off
    auto option_list = cxxopts::Options("serde_gen",
                                        "serde_gen - serde-cpp serialization and deserialization generator.\n");
    option_list.add_options()
            ("h,help", "display this help and exit")
            ("V,version", "display version information and exit")
            ("v,verbose", "be verbose when parsing")
            ("F,fatal_errors", "abort program when a parser error occurs, instead of doing error correction");
    option_list.add_options("compilation")
            ("s,source", "the file that is being parsed", cxxopts::value<std::string>())
            ("o,output", "the output file that will be generated", cxxopts::value<std::string>())
            ("D,database_dir",
             "set the directory where a 'compile_commands.json' file is located containing build information",
             cxxopts::value<std::string>())
            ("d,database_file",
             "set the file name whose configuration will be used regardless of the current file name",
             cxxopts::value<std::string>())
            ("I,include_directory", "add directory to include search path",
             cxxopts::value<std::vector<std::string>>());
    // clang-format on
    return option_list;
}

bool handle_help_version(const cxxopts::Options &option_list,
                         const cxxopts::ParseResult &options) {
    if (options.count("help")) {
        std::cout << option_list.help() << std::endl;
        return true;
    }
    if (options.count("version")) {
        std::cout << "serde_gen 0.1.0\n\n"
                  << "Using libclang version " << CPPAST_CLANG_VERSION_STRING << '\n';
        return true;
    }
    return false;
}

auto validate_options(const cxxopts::ParseResult &options) -> int {
    int ret = 0;
    if (!options.count("source") || options["source"].as<std::string>().empty()) {
        std::cerr << "missing --source argument\n";
        ret = 1;
    }
    if (!options.count("output") || options["output"].as<std::string>().empty()) {
        std::cerr << "missing --output argument\n";
        ret = 1;
    }
    return ret;
}

static auto create_clang_compilation_config(const cxxopts::ParseResult &options) {
    cppast::libclang_compile_config config;
    if (options.count("database_dir")) {
        auto db = cppast::libclang_compilation_database(options["database_dir"].as<std::string>());
        std::string db_file = "compile_commands.json";
        if (options.count("database_file")) {
            db_file = options["database_file"].as<std::string>();
        }
        config = cppast::libclang_compile_config(db, db_file);
    }
    return config;
}

static auto add_compilation_flags(const cxxopts::ParseResult &_options,
                                  cppast::libclang_compile_config &clang_cfg) {
    cppast::compile_flags flags;
    clang_cfg.set_flags(cppast::cpp_standard::cpp_17, flags);
}

static auto add_include_directories(const cxxopts::ParseResult &options,
                                    cppast::libclang_compile_config &clang_cfg) {
    if (options.count("include_directory")) {
        for (auto &include: options["include_directory"].as<std::vector<std::string>>())
            clang_cfg.add_include_dir(include);
    }
}

static auto init_clang_compilation_config(const cxxopts::ParseResult &options) {
    auto clang_cfg = create_clang_compilation_config(options);
    add_compilation_flags(options, clang_cfg);
    add_include_directories(options, clang_cfg);
    return clang_cfg;
}

static auto init_diagnostic_logger(const cxxopts::ParseResult &options) {
    cppast::stderr_diagnostic_logger logger;
    auto verbose = options.count("verbose");
    if (verbose)
        logger.set_verbose(true);
    return logger;
}

static auto touch_file(const std::string &filename) {
    auto out_base_path = std::filesystem::path(filename).remove_filename();
    std::filesystem::create_directories(out_base_path);
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open output file: " << std::strerror(errno) << std::endl;
        return false;
    }
    file.close();
    return true;
}

static auto parse_source_to_ast(const cxxopts::ParseResult &options)
-> std::unique_ptr<cppast::cpp_file> {
    const auto &source_filename = options["source"].as<std::string>();
    const auto fatal_errors = options.count("fatal_errors");
    auto clang_cfg = init_clang_compilation_config(options);
    auto logger = init_diagnostic_logger(options);
    auto src_ast = serde_gen::parse_file(clang_cfg, logger, source_filename, fatal_errors);
    return src_ast;
}

auto run_serde_generator(const cxxopts::ParseResult &options) -> int {
    // Pre-create output file for handling #include of generated serde file while parsing source
    const auto &output_filename = options["output"].as<std::string>();
    if (!touch_file(output_filename))
        return 3;

    auto src_ast = parse_source_to_ast(options);
    if (!src_ast)
        return 2;

    if (options.count("verbose"))
        serde_gen::print_ast(std::cout, *src_ast);

    std::ofstream outfile(output_filename);
    serde_gen::generate_serde(outfile, *src_ast);

    return 0;
}

}  // namespace serde_gen::init