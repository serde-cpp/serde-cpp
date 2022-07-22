#pragma once

#include <vector>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct Deserialize<std::vector> {
  template<typename... U>
  static void deserialize(Deserializer& de, std::vector<U...>& vec) {
    de.deserialize_seq_begin();
    for (auto& v : vec)
      de.deserialize(v);
    de.deserialize_seq_end();
  }
};

} // namespace serde

