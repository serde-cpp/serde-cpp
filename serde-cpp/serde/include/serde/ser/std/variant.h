#pragma once

#include <variant>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeT<std::variant> {
  template<typename... Ts>
  static void serialize(Serializer& ser, const std::variant<Ts...>& variant) {
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
