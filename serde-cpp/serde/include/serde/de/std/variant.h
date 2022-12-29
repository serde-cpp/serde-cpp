#pragma once

#include <variant>
#include <utility>
#include <type_traits>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::variant> {
  template<typename... Ts>
  static void deserialize(Deserializer& de, std::variant<Ts...>& variant) {
    de.deserialize_map_begin();
    size_t index = 0;
    de.deserialize_map_key(index);
    deserialize_expand(de, variant, index, std::make_integer_sequence<size_t, sizeof...(Ts)>());
    de.deserialize_map_end();
  }

private:

  template<typename Variant, size_t... Is>
  static void deserialize_expand(Deserializer& de, Variant& variant, size_t index, std::integer_sequence<size_t, Is...>) {
    (deserialize_index<Is>(de, variant, index), ...);
  }

  template<size_t I, typename Variant>
  static void deserialize_index(Deserializer& de, Variant& variant, size_t index) {
    if (index == I) {
      deserialize_variant<I>(de, variant);
    }
  }

  template<size_t I, typename... Ts>
  static void deserialize_variant(Deserializer& de, std::variant<Ts...>& variant) {
    using Type = std::remove_reference_t<decltype(std::get<I>(variant))>;
    Type value;
    de.deserialize_map_value(value);
    variant = std::move(value);
  }
};

} // namespace serde

