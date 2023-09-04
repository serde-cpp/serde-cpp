#include <fstream>

#include <cppast/code_generator.hpp>
#include <cppast/cpp_entity_kind.hpp>
#include <cppast/cpp_forward_declarable.hpp>
#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_namespace.hpp>
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>

#include "code_generator.h"
#include "common.h"

namespace serde_gen {

class Generator {
    std::ostream& os;

   public:
    Generator(std::ostream& os) : os(os) {}

    Generator& file_header()
    {
        os << R"(/*
* Serde-cpp generated header file.
* DO NOT EDIT!!
*/)";
        return *this;
    }

    Generator& line_break(size_t count = 1)
    {
        while (count--)
            os << "\n";
        return *this;
    }

    Generator& include_system(const char* header)
    {
        os << "#include <" << header << ">\n";
        return *this;
    }

    Generator& include_local(const char* header)
    {
        os << "#include \"" << header << "\"\n";
        return *this;
    }

    Generator& namespace_open(const char* ns)
    {
        os << "namespace " << ns << " {\n";
        return *this;
    }

    Generator& namespace_close(const char* ns)
    {
        os << "}  // namespace " << ns << '\n';
        return *this;
    }

    Generator& struct_serialize_begin(const char* name)
    {
        os << R"("
// Serialize specialization
template<typename T>
struct Serialize<T, std::enable_if_t<std::is_same_v<T, )"
           << name << R"(>>> { )";
    }

    Generator& struct_serialize_end(const char* name)
    {
        os << "};  // Serialize " << name << '\n';
        return *this;
    }

    Generator& struct_static_serialize_fn()
    {
        os << R"(static void serialize(Serializer& ser, const T& val) )";
        return *this;
    }
};

void generate_serde(std::ofstream& outfile, const cppast::cpp_file& file)
{

    auto gen = Generator(outfile);
    gen.file_header()
        .line_break()
        .include_system("string")
        .include_local("serde/serde.h")
        .include_local("serde/std/string.h")
        .line_break()
        .namespace_open("serde")
        .line_break()
        .struct_serialize_begin("Options")
        .struct_static_serialize_fn()
        .block_begin()
        .Serializer_serialize_struct_begin()
        .Serializer_serialize_struct_field("debug", "debug")
        .Serializer_serialize_struct_field("line", "line")
        .Serializer_serialize_struct_field("func", "func")
        .Serializer_serialize_struct_end()
        .block_end()
        .struct_serialize_end("Options")
        .line_break()
        .namespace_close("serde");

    outfile << R"(/*
* Serde-cpp generated header file.
* DO NOT EDIT!!
*/

#include <string>
#include <serde/serde.h>
#include <serde/std/string.h>
)";

    cppast::visit(
        file,
        [](const cppast::cpp_entity& e) {
            return (e.kind() == cppast::cpp_entity_kind::class_t && cppast::is_definition(e) &&
                    cppast::has_attribute(e, "serde")) ||
                   e.kind() == cppast::cpp_entity_kind::namespace_t;
        },
        [&](const cppast::cpp_entity& e, cppast::visitor_info info) {
            if (e.kind() == cppast::cpp_entity_kind::class_t && !info.is_old_entity()) {
                auto& class_ = static_cast<const cppast::cpp_class&>(e);

                outfile << "\n" << CodeGeneratorFromAst(e).str() << "\n";

                outfile << R"(
namespace serde {
// Serialize specialization
template<typename T>
struct Serialize<T, std::enable_if_t<std::is_same_v<T, )"
                        << e.name() << R"(>>> {
  static void serialize(Serializer& ser, const T& val) {
    ser.serialize_struct_begin();
)";
                // serialize member variables
                for (auto& member : class_) {
                    if (member.kind() == cppast::cpp_entity_kind::member_variable_t) {
                        const auto& member_var =
                            static_cast<const cppast::cpp_member_variable&>(member);
                        outfile << "      ser.serialize_struct_field(\"" << member_var.name()
                                << "\", val." << member_var.name() << ");\n";
                    }
                }
                outfile <<
                    R"(    ser.serialize_struct_end();
  }
};
// Deserialize specialization
template<typename T>
struct Deserialize<T, std::enable_if_t<std::is_same_v<T, )"
                        << e.name() << R"(>>> {
  static void deserialize(Deserializer& de, T& val) {
    de.deserialize_struct_begin();
)";
                // deserialize member variables
                for (auto& member : class_) {
                    if (member.kind() == cppast::cpp_entity_kind::member_variable_t) {
                        const auto& member_var =
                            static_cast<const cppast::cpp_member_variable&>(member);
                        outfile << "      de.deserialize_struct_field(\"" << member_var.name()
                                << "\", val." << member_var.name() << ");\n";
                    }
                }
                outfile <<
                    R"(    de.deserialize_struct_end();
  }
};
} // namespace serde

)";
            }
        });
}

}  // namespace serde_gen
