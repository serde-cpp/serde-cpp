#pragma once

#include <array>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeTN<std::array> {
  template<typename U, auto N>
  static void deserialize(Deserializer& de, std::array<U, N>& array) {
    de.deserialize_seq_begin();
    for (auto& e : array)
      de.deserialize(e);
    de.deserialize_seq_end();
  }
};

} // namespace serde


