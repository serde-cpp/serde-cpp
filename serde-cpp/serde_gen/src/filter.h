#pragma once

#include <cppast/cpp_entity_kind.hpp>
#include <cppast/cpp_forward_declarable.hpp>

namespace serde_gen {

class Filter final {
    /// Static class
    Filter() = delete;

   public:
    inline static bool cpp_entities_with_serde_attr(const cppast::cpp_entity& e)
    {
        return e.kind() == cppast::cpp_entity_kind::class_t && cppast::is_definition(e) &&
               cppast::has_attribute(e, "serde");
    }
};

}  // namespace serde_gen
