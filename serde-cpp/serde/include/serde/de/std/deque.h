#pragma once

#include <deque>

#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct Deserialize<std::deque> {
  template<typename... U>
  static void deserialize(Deserializer& de, std::deque<U...>& deque) {
    size_t size = 0;
    de.deserialize_seq_begin();
    de.deserialize_seq_size(size);
    deque.resize(size);
    for (auto& e : deque)
      de.deserialize(e);
    de.deserialize_seq_end();
  }
};

} // namespace serde

