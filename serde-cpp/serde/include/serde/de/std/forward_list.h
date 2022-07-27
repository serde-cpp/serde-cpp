#pragma once

#include <forward_list>

#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct Deserialize<std::forward_list> {
  template<typename... U>
  static void deserialize(Deserializer& de, std::forward_list<U...>& list) {
    size_t size = 0;
    de.deserialize_seq_begin();
    de.deserialize_seq_size(size);
    list.resize(size);
    for (auto& e : list)
      de.deserialize(e);
    de.deserialize_seq_end();
  }
};

} // namespace serde

