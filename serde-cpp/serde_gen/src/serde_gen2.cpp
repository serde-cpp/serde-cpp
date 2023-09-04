#include <fstream>
#include <iomanip>

#include <cppast/code_generator.hpp>
#include <cppast/cpp_entity_kind.hpp>
#include <cppast/cpp_forward_declarable.hpp>
#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_namespace.hpp>
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>

#include "code_generator.h"
#include "common.h"
#include "gen.h"

namespace serde_gen {

static bool filter_serde_classes(const cppast::cpp_entity& e)
{
    return e.kind() == cppast::cpp_entity_kind::class_t && cppast::is_definition(e) &&
           cppast::has_attribute(e, "serde");
}

void generate_serde(std::ofstream& outfile, const cppast::cpp_file& file)
{
    using namespace gen;

    auto gen = Generator(outfile);
    gen << FileHeader();
    gen << LineBreak();
    gen << IncludeLocal("serde/serde.h");
    gen << IncludeLocal("serde/std/string.h");
    gen << LineBreak();

    cppast::visit(
        file, filter_serde_classes, [&](const cppast::cpp_entity& e, cppast::visitor_info info) {
            if (e.kind() == cppast::cpp_entity_kind::class_t && !info.is_old_entity()) {
                auto& class_ = static_cast<const cppast::cpp_class&>(e);

                outfile << code_generator(e).str() << "\n";

                gen << LineBreak();
                gen << Namespace("serde");
                gen << BlockBegin();
                gen << LineBreak();
                gen << StructSerialize(std::string(e.name()));
                gen << BlockBegin();
                gen << SerializeFunction();
                gen << BlockBegin();
                gen << SERIALIZE_STRUCT_BEGIN;

                // serialize member variables
                for (auto& member : class_) {
                    if (member.kind() == cppast::cpp_entity_kind::member_variable_t) {
                        const auto& member_var =
                            static_cast<const cppast::cpp_member_variable&>(member);
                        outfile << "      ser.serialize_struct_field(\"" << member_var.name()
                                << "\", val." << member_var.name() << ");\n";
                    }
                }

                gen << SERIALIZE_STRUCT_END;
                gen << BlockEnd();           // serialize func
                gen << BlockEndSemiColon();  // Serialize struct
                gen << LineBreak();
                gen << BlockEnd();  // namespace serde
            }
        });
}

}  // namespace serde_gen
