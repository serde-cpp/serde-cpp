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

#include "cppast_code_generator.h"
#include "generate.h"
#include "common.h"
#include "filter.h"
#include "gen.h"

namespace serde_gen {

void generate_serde_for_file(std::ostream& output, const cppast::cpp_file& file)
{
    using namespace gen;

    auto gen = Generator();
    gen.add_header(FileHeader());
    gen.add_include_local("serde/serde.h");
    gen.add_include_local("serde/std/string.h");

    cppast::visit(file, Filter::cpp_entities_with_serde_attr, [&](const auto& e, const auto& info) {
        generate_serde_for_entity(gen, e, info);
    });

    gen.write(output);
}

void generate_serde_for_entity(gen::Generator& gen, const cppast::cpp_entity& e,
                               const cppast::visitor_info& info)
{
    if (e.kind() == cppast::cpp_entity_kind::class_t) {
        if (!info.is_old_entity()) {  // meaning: not visited yet
            generate_serde_for_class(gen, e, info);
        }
    }
    else {
        auto entity_decl = CppastCodeGenerator(e).str();
        fprintf(stderr, "Unhandled cpp entity: %s\n", entity_decl.c_str());
    }
}

void generate_serde_for_class(gen::Generator& gen, const cppast::cpp_entity& e,
                              const cppast::visitor_info& info)
{
    using namespace gen;

    auto entity_decl = CppastCodeGenerator(e).str();
    gen.add(GenString(std::move(entity_decl)));  // forward-declaration
                                                 // TODO: support namespaces
    gen.add(LineBreak(2));
    gen.add(Namespace("serde"));
    gen.add(BlockBeginNL());
    gen.add(LineBreak());

    generate_struct_serialize(gen, e, info);
    generate_struct_deserialize(gen, e, info);

    gen.add(BlockEnd());
    gen.add(CommentInline("namespace serde\n"));
    gen.add(LineBreak());
}

void generate_struct_serialize(gen::Generator& gen, const cppast::cpp_entity& e,
                               const cppast::visitor_info& info)
{
    using namespace gen;

    const auto& cpp_class = static_cast<const cppast::cpp_class&>(e);

    gen.add(StructSerializeBegin(std::string(e.name())));
    gen.add(StaticMethodSerializeBegin());
    gen.add(ApiSerializeStructBegin());

    for (const auto& member : cpp_class) {
        if (member.kind() == cppast::cpp_entity_kind::member_variable_t) {
            const auto& member_var = static_cast<const cppast::cpp_member_variable&>(member);
            gen.add(ApiSerializeStructField(member_var.name(), member_var.name()));
        }
    }

    gen.add(ApiSerializeStructEnd());
    gen.add(StaticMethodSerializeEnd());
    gen.add(StructSerializeEnd());
    gen.add(LineBreak());
}

void generate_struct_deserialize(gen::Generator& gen, const cppast::cpp_entity& e,
                                 const cppast::visitor_info& info)
{
    using namespace gen;

    const auto& cpp_class = static_cast<const cppast::cpp_class&>(e);

    gen.add(StructDeserializeBegin(std::string(e.name())));
    gen.add(StaticMethodDeserializeBegin());
    gen.add(ApiDeserializeStructBegin());

    for (const auto& member : cpp_class) {
        if (member.kind() == cppast::cpp_entity_kind::member_variable_t) {
            const auto& member_var = static_cast<const cppast::cpp_member_variable&>(member);
            gen.add(ApiDeserializeStructField(member_var.name(), member_var.name()));
        }
    }

    gen.add(ApiDeserializeStructEnd());
    gen.add(StaticMethodDeserializeEnd());
    gen.add(StructDeserializeEnd());
    gen.add(LineBreak());
}

}  // namespace serde_gen
