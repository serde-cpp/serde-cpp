#pragma once

#include <optional>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::optional> {
  template<typename T>
  static void deserialize(Deserializer& de, std::optional<T>& val) {
    bool some = false;
    de.deserialize_is_some(some);
    if (some) {
      typename std::optional<T>::value_type inner_val;
      de.deserialize(inner_val);
      val = std::move(inner_val);
    }
    else {
      de.deserialize_none();
      val = std::nullopt;
    }
  }
};

} // namespace serde

