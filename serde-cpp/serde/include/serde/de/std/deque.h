#pragma once

#include <deque>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::deque> {
  template<typename T, typename Alloc>
  static void deserialize(Deserializer& de, std::deque<T, Alloc>& deque) {
    size_t size = 0;
    de.deserialize_seq_size(size);
    de.deserialize_seq_begin();
    deque.resize(size);
    for (auto& e : deque)
      de.deserialize(e);
    de.deserialize_seq_end();
  }
};

} // namespace serde

