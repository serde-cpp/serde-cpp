#include <fstream>

#include <cppast/code_generator.hpp>  // for generate_code()
#include <cppast/cpp_entity_kind.hpp> // for the cpp_entity_kind definition
#include <cppast/cpp_forward_declarable.hpp> // for is_definition()
#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_namespace.hpp> // for cpp_namespace
#include <cppast/libclang_parser.hpp> // for libclang_parser, libclang_compile_config, cpp_entity,...
#include <cppast/visitor.hpp> // for visit()

#include "code_generator.h"
#include "common.h"

void generate_serde(std::ofstream &outfile, const cppast::cpp_file &file) {
  outfile << R"(/* Auto-generated serde-cpp file! DO NOT EDIT!!! */

#include <string>
#include <serde/serde.h>
#include <serde/std/string.h>
)";

  cppast::visit(
      file,
      [](const cppast::cpp_entity &e) {
        // only visit non-templated class definitions that have the attribute
        // set
        return (e.kind() == cppast::cpp_entity_kind::class_t &&
                cppast::is_definition(e) && cppast::has_attribute(e, "serde"))
               // or all namespaces
               || e.kind() == cppast::cpp_entity_kind::namespace_t;
      },
      [&](const cppast::cpp_entity &e, cppast::visitor_info info) {
        if (e.kind() == cppast::cpp_entity_kind::class_t &&
            !info.is_old_entity()) {
          auto &class_ = static_cast<const cppast::cpp_class &>(e);

          outfile << "\n" << code_generator(e).str() << "\n";

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
          for (auto &member : class_) {
            if (member.kind() == cppast::cpp_entity_kind::member_variable_t) {
              const auto &member_var =
                  static_cast<const cppast::cpp_member_variable &>(member);
              outfile << "      ser.serialize_struct_field(\""
                      << member_var.name() << "\", val." << member_var.name()
                      << ");\n";
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
          for (auto &member : class_) {
            if (member.kind() == cppast::cpp_entity_kind::member_variable_t) {
              const auto &member_var =
                  static_cast<const cppast::cpp_member_variable &>(member);
              outfile << "      de.deserialize_struct_field(\""
                      << member_var.name() << "\", val." << member_var.name()
                      << ");\n";
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
