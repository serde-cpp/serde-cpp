#pragma once

#include <memory>
#include <iostream>
#include <cppast/cpp_entity.hpp>

#include "gen.h"

namespace serde_gen {

/// Generate serde for an entire parsed file
void generate_serde_for_file(std::ostream& outfile, const cppast::cpp_file& file);

/// Generate serde for a cpp_entity
void generate_serde_for_entity(gen::Generator& gen, const cppast::cpp_entity& e,
                               const cppast::visitor_info& info);

/// Generate serde for a class/struct with serde attribute
void generate_serde_for_class(gen::Generator& gen, const cppast::cpp_entity& e,
                              const cppast::visitor_info& info);

/// Generate struct Serialize for a given type
void generate_struct_serialize(gen::Generator& gen, const cppast::cpp_entity& e,
                               const cppast::visitor_info& info);

/// Generate struct Deserialize for a given type
void generate_struct_deserialize(gen::Generator& gen, const cppast::cpp_entity& e,
                                 const cppast::visitor_info& info);

}  // namespace serde_gen
