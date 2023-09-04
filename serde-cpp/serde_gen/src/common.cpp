#include "common.h"

#include <iostream>

#include <cppast/code_generator.hpp>
#include <cppast/cpp_entity_kind.hpp>
#include <cppast/cpp_forward_declarable.hpp>
#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_namespace.hpp>
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>

#include "code_generator.h"

namespace serde_gen {

auto parse_file(const cppast::libclang_compile_config& config,
                const cppast::diagnostic_logger& logger, const std::string& filename,
                bool fatal_error) -> std::unique_ptr<cppast::cpp_file>
{
    // the entity index is used to resolve cross references in the AST
    // we don't need that, so it will not be needed afterwards
    cppast::cpp_entity_index idx;
    // the parser is used to parse the entity
    // there can be multiple parser implementations
    cppast::libclang_parser parser(type_safe::ref(logger));
    // parse the file
    auto file = parser.parse(idx, filename, config);
    if (fatal_error && parser.error())
        return nullptr;
    return file;
}

// prints the AST entry of a cpp_entity (base class for all entities),
// will only print a single line
void print_entity(std::ostream& out, const cppast::cpp_entity& e)
{
    // print name and the kind of the entity
    if (!e.name().empty())
        out << e.name();
    else
        out << "<anonymous>";
    out << " (" << cppast::to_string(e.kind()) << ")";

    // print whether or not it is a definition
    if (cppast::is_definition(e))
        out << " [definition]";

    // print number of attributes
    if (!e.attributes().empty())
        out << " [" << e.attributes().size() << " attribute(s)]";

    if (e.kind() == cppast::cpp_entity_kind::language_linkage_t)
        // no need to print additional information for language linkages
        out << '\n';
    else if (e.kind() == cppast::cpp_entity_kind::namespace_t) {
        // cast to cpp_namespace
        auto& ns = static_cast<const cppast::cpp_namespace&>(e);
        // print whether or not it is inline
        if (ns.is_inline())
            out << " [inline]";
        out << '\n';
    }
    else {
        // print generated code
        out << ": `" << CodeGeneratorFromAst(e).str() << '`' << '\n';
    }
}

// prints the AST of a file
void print_ast(std::ostream& out, const cppast::cpp_file& file)
{
    // print file name
    out << "AST for '" << file.name() << "':\n";
    std::string prefix;  // the current prefix string
    // recursively visit file and all children
    cppast::visit(file, [&](const cppast::cpp_entity& e, cppast::visitor_info info) {
        if (e.kind() == cppast::cpp_entity_kind::file_t || cppast::is_templated(e) ||
            cppast::is_friended(e))
            // no need to do anything for a file,
            // templated and friended entities are just proxies, so skip those as
            // well return true to continue visit for children
            return true;
        else if (info.event == cppast::visitor_info::container_entity_exit) {
            // we have visited all children of a container,
            // remove prefix
            prefix.pop_back();
            prefix.pop_back();
        }
        else {
            out << prefix;  // print prefix for previous entities
            // calculate next prefix
            if (info.last_child) {
                if (info.event == cppast::visitor_info::container_entity_enter)
                    prefix += "  ";
                out << "+-";
            }
            else {
                if (info.event == cppast::visitor_info::container_entity_enter)
                    prefix += "| ";
                out << "|-";
            }

            print_entity(out, e);
        }

        return true;
    });
}

}  // namespace serde_gen
