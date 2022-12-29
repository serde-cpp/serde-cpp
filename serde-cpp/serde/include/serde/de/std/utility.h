#pragma once

#include <utility>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::pair> {
  template<typename T1, typename T2>
  static void deserialize(Deserializer& de, std::pair<T1, T2>& pair) {
    de.deserialize_struct_begin();
    de.deserialize_struct_field("first", pair.first);
    de.deserialize_struct_field("second", pair.second);
    de.deserialize_struct_end();
  }
};

} // namespace serde

