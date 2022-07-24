#pragma once

#include <vector>

#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct Deserialize<std::vector> {
  template<typename... U>
  static void deserialize(Deserializer& de, std::vector<U...>& vec) {
    size_t size = 0;
    de.deserialize_seq_begin();
    de.deserialize_seq_size(size);
    vec.resize(size);
    for (auto& e : vec)
      de.deserialize(e);
    de.deserialize_seq_end();
  }
};

} // namespace serde

