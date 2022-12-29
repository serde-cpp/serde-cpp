#pragma once

#include <set>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::set> {
  template<typename Key, typename Cmp, typename Alloc>
  static void deserialize(Deserializer& de, std::set<Key, Cmp, Alloc>& set) {
    size_t size = 0;
    set.clear();
    de.deserialize_seq_size(size);
    de.deserialize_seq_begin();
    for (size_t i = 0; i < size; i++) {
      Key key;
      de.deserialize(key);
      set.emplace(std::move(key));
    }
    de.deserialize_seq_end();
  }
};

template<>
struct DeserializeT<std::multiset> {
  template<typename Key, typename Cmp, typename Alloc>
  static void deserialize(Deserializer& de, std::multiset<Key, Cmp, Alloc>& multiset) {
    size_t size = 0;
    multiset.clear();
    de.deserialize_seq_size(size);
    de.deserialize_seq_begin();
    for (size_t i = 0; i < size; i++) {
      Key key;
      de.deserialize(key);
      multiset.emplace(std::move(key));
    }
    de.deserialize_seq_end();
  }
};

} // namespace serde

