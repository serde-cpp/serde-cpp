#pragma once

#include <variant>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::variant> {
  template<typename... U>
  static void serialize(Serializer& ser, const std::variant<U...>& variant) {
    size_t index = variant.index();
    ser.serialize_map_begin();
    ser.serialize_map_key(index);
    std::visit([&](const auto& val) {
      ser.serialize_map_value(val);
    }, variant);
    ser.serialize_map_end();
  }
};

} // namespace serde

