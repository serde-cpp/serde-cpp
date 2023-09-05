#include <fstream>
#include <iomanip>

#include <cppast/code_generator.hpp>
#include <cppast/cpp_entity_kind.hpp>
#include <cppast/cpp_forward_declarable.hpp>
#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_namespace.hpp>
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>
#include <cppast/cpp_class.hpp>

#include "code_generator.h"
#include "common.h"
#include "gen.h"
#include "filters.h"

namespace serde_gen {

static void generate_struct_serialize(gen::Generator& gen, const cppast::cpp_entity& e,
                                      const cppast::visitor_info& info)
{
    using namespace gen;

    auto& class_ = static_cast<const cppast::cpp_class&>(e);

    gen << StructSerialize(std::string(e.name()));
    gen << BlockBeginNL();
    gen << StaticFunctionSerialize();
    gen << BlockBeginNL();
    gen << SERIALIZE_STRUCT_BEGIN;

    // serialize member variables
    for (auto& member : class_) {
        if (member.kind() == cppast::cpp_entity_kind::member_variable_t) {
            const auto& member_var = static_cast<const cppast::cpp_member_variable&>(member);
            gen.os << "ser.serialize_struct_field(\"" << member_var.name() << "\", val."
                   << member_var.name() << ");\n";
        }
    }

    gen << SERIALIZE_STRUCT_END;
    gen << BlockEndNL();
    gen << BlockEndSemiColonNL();
    gen << LineBreak();
}

static void generate_struct_deserialize(gen::Generator& gen, const cppast::cpp_entity& e,
                                        const cppast::visitor_info& info)
{
    using namespace gen;

    auto& class_ = static_cast<const cppast::cpp_class&>(e);

    gen << StructDeserialize(std::string(e.name()));
    gen << BlockBeginNL();
    gen << StaticFunctionDeserialize();
    gen << BlockBeginNL();
    gen << DESERIALIZE_STRUCT_BEGIN;

    // deserialize member variables
    for (auto& member : class_) {
        if (member.kind() == cppast::cpp_entity_kind::member_variable_t) {
            const auto& member_var = static_cast<const cppast::cpp_member_variable&>(member);
            gen.os << "de.deserialize_struct_field(\"" << member_var.name() << "\", val."
                   << member_var.name() << ");\n";
        }
    }

    gen << DESERIALIZE_STRUCT_END;
    gen << BlockEndNL();
    gen << BlockEndSemiColonNL();
    gen << LineBreak();
}

static void generate_serde_for_class(gen::Generator& gen, const cppast::cpp_entity& e,
                                     const cppast::visitor_info& info)
{
    using namespace gen;

    gen << GenString(std::string(CodeGeneratorFromAst(e).str()));
    gen << LineBreak(2);
    gen << Namespace("serde") << BlockBeginNL();
    gen << LineBreak();

    generate_struct_serialize(gen, e, info);
    generate_struct_deserialize(gen, e, info);

    gen << BlockEnd() << CommentInline("namespace serde\n");
    gen << LineBreak();
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

    cppast::visit(file, filter_class_entities_with_serde_attr,
                  [&](const cppast::cpp_entity& e, const cppast::visitor_info& info) {
                      if (e.kind() == cppast::cpp_entity_kind::class_t && !info.is_old_entity()) {
                          generate_serde_for_class(gen, e, info);
                      }
                  });
}

}  // namespace serde_gen
