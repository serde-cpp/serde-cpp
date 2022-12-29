#pragma once

#include <map>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::map> {
  template<typename Key, typename Value, typename Cmp, typename Alloc>
  static void deserialize(Deserializer& de, std::map<Key, Value, Cmp, Alloc>& map) {
    size_t size = 0;
    map.clear();
    de.deserialize_map_size(size);
    de.deserialize_map_begin();
    for (size_t i = 0; i < size; i++) {
      Key key; Value value;
      de.deserialize_map_entry(key, value);
      map.emplace(std::move(key), std::move(value));
    }
    de.deserialize_map_end();
  }
};

template<>
struct DeserializeT<std::multimap> {
  template<typename Key, typename Value, typename Cmp, typename Alloc>
  static void deserialize(Deserializer& de, std::multimap<Key, Value, Cmp, Alloc>& multimap) {
    size_t size = 0;
    multimap.clear();
    de.deserialize_map_size(size);
    de.deserialize_map_begin();
    for (size_t i = 0; i < size; i++) {
      Key key; Value value;
      de.deserialize_map_entry(key, value);
      multimap.emplace(std::move(key), std::move(value));
    }
    de.deserialize_map_end();
  }
};

} // namespace serde

