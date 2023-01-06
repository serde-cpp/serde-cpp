#pragma once

#include <memory>
#include <iostream>
#include <cppast/cpp_entity.hpp>

// parse file
auto parse_file(const cppast::libclang_compile_config &config,
                const cppast::diagnostic_logger &logger,
                const std::string &filename, bool fatal_error)
    -> std::unique_ptr<cppast::cpp_file>;

// prints the AST entry of a cpp_entity (base class for all entities),
// will only print a single line
void print_entity(std::ostream& out, const cppast::cpp_entity& e);

// prints the AST of a file
void print_ast(std::ostream &out, const cppast::cpp_file &file);

// generate serde header
void generate_serde(std::ofstream &outfile, const cppast::cpp_file &file);
