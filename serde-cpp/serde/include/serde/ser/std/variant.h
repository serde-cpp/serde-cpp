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
    std::visit(overloaded {
      ([&](const U& val){ ser.serialize(val); }, ...),
    }, variant);
    ser.serialize_map_end();
  }

  template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
  template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
};

} // namespace serde

