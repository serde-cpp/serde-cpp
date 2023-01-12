#include <filesystem>
#include <fstream>
#include <iostream>
#include <tuple>

#include <cxxopts.hpp>
#include <cppast/libclang_parser.hpp>

#include "common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Internal funcs
///////////////////////////////////////////////////////////////////////////////////////////////////

static auto create_option_list() -> cxxopts::Options
{
    auto option_list = cxxopts::Options(
        "serde_attr", "serde_attr - Serde attribute parser and serialization generator.\n");
    // clang-format off
    option_list.add_options()
        ("h,help", "display this help and exit")
        ("version", "display version information and exit")
        ("v,verbose", "be verbose when parsing")
        ("fatal_errors", "abort program when a parser error occurs, instead of doing error correction");
    option_list.add_options("compilation")
        ("source", "the file that is being parsed", cxxopts::value<std::string>())
        ("output", "the output file that will be generated", cxxopts::value<std::string>())
        ("database_dir", "set the directory where a 'compile_commands.json' file is located containing build information", cxxopts::value<std::string>())
        ("database_file", "set the file name whose configuration will be used regardless of the current file name", cxxopts::value<std::string>())
        ("I,include_directory", "add directory to include search path", cxxopts::value<std::vector<std::string>>());
    // clang-format on
    return option_list;
}

static auto handle_help_version(const cxxopts::Options& option_list,
                                const cxxopts::ParseResult& options)
{
    if (options.count("help")) {
        std::cout << option_list.help() << std::endl;
        return 1;
    }
    if (options.count("version")) {
        std::cout << "serde_gen 0.1.0\n\n"
                  << "Using libclang version " << CPPAST_CLANG_VERSION_STRING << '\n';
        return 1;
    }
    return 0;
}

static auto validate_options(const cxxopts::ParseResult& options)
{
    if (!options.count("source") || options["source"].as<std::string>().empty()) {
        std::cerr << "missing --source argument\n";
        return 1;
    }
    if (!options.count("output") || options["output"].as<std::string>().empty()) {
        std::cerr << "missing --output argument\n";
        return 1;
    }
    return 0;
}

static auto create_clang_compilation_config(const cxxopts::ParseResult& options)
{
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

static auto add_compilation_flags(const cxxopts::ParseResult& _options,
                                  cppast::libclang_compile_config clang_cfg)
{
    cppast::compile_flags flags;
    clang_cfg.set_flags(cppast::cpp_standard::cpp_17, flags);
    clang_cfg.fast_preprocessing(true);
}

static auto add_include_directories(const cxxopts::ParseResult& options,
                                    cppast::libclang_compile_config clang_cfg)
{
    if (options.count("include_directory")) {
        for (auto& include : options["include_directory"].as<std::vector<std::string>>())
            clang_cfg.add_include_dir(include);
    }
}

static auto init_clang_compilation_config(const cxxopts::ParseResult& options)
{
    auto clang_cfg = create_clang_compilation_config(options);
    add_compilation_flags(options, clang_cfg);
    add_include_directories(options, clang_cfg);
    return clang_cfg;
}

static auto init_diagnostic_logger(const cxxopts::ParseResult& options)
{
    cppast::stderr_diagnostic_logger logger;
    auto verbose = options.count("verbose");
    if (verbose)
        logger.set_verbose(true);
    return logger;
}

static auto create_file(const std::string& filename)
{
    auto out_base_path = std::filesystem::path(filename).remove_filename();
    std::filesystem::create_directories(out_base_path);
    std::ofstream file(filename);
    if (!file.is_open())
        abort();
    file.close();
}

static auto parse_source(const cxxopts::ParseResult& options)
    -> std::tuple<int, std::unique_ptr<cppast::cpp_file>>
{
    const auto& source_filename = options["source"].as<std::string>();
    const auto fatal_errors = options.count("fatal_errors");
    auto clang_cfg = init_clang_compilation_config(options);
    auto logger = init_diagnostic_logger(options);
    auto src_ast = serde_gen::parse_file(clang_cfg, logger, source_filename, fatal_errors);
    if (!src_ast)
        return { 2, nullptr };
    return { 0, std::move(src_ast) };
}

static auto run_generator(const cxxopts::ParseResult& options)
{
    // Pre-create output file for handling #include of generated serde file while parsing source
    const auto& output_filename = options["output"].as<std::string>();
    create_file(output_filename);

    // Parse source to AST
    auto [rv, src_ast] = parse_source(options);
    if (rv)
        return rv;

    // Debug log AST
    if (options.count("verbose"))
        serde_gen::print_ast(std::cout, *src_ast);

    // Generate serde header
    std::ofstream outfile(output_filename);
    serde_gen::generate_serde(outfile, *src_ast);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    int ret = 0;
    auto option_list = create_option_list();
    const auto options = option_list.parse(argc, argv);

    ret = handle_help_version(option_list, options);
    if (ret == 1)
        return 0;

    ret = validate_options(options);
    if (ret)
        return ret;

    ret = run_generator(options);
    if (ret)
        return ret;

    return 0;
}
