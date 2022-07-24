#pragma once

#include <unordered_set>

#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct Deserialize<std::unordered_set> {
  template<typename Key, typename... U>
  static void deserialize(Deserializer& de, std::unordered_set<Key, U...>& set) {
    size_t size = 0;
    set.clear();
    de.deserialize_seq_begin();
    de.deserialize_seq_size(size);
    for (int i = 0; i < size; i++) {
      Key key;
      de.deserialize(key);
      set.insert(std::move(key));
    }
    de.deserialize_seq_end();
  }
};

} // namespace serde




