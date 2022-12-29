#pragma once

#include <vector>

#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::vector> {
  template<typename T, typename Alloc>
  static void deserialize(Deserializer& de, std::vector<T, Alloc>& vec) {
    size_t size = 0;
    de.deserialize_seq_size(size);
    de.deserialize_seq_begin();
    vec.resize(size);
    for (auto& e : vec)
      de.deserialize(e);
    de.deserialize_seq_end();
  }
};

} // namespace serde

