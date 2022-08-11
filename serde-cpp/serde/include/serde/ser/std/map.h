#pragma once

#include <map>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeT<std::map> {
  template<typename Key, typename Value, typename Cmp, typename Alloc>
  static void serialize(Serializer& ser, const std::map<Key, Value, Cmp, Alloc>& map) {
    ser.serialize_map_begin();
    for (auto& it : map)
      ser.serialize_map_entry(it.first, it.second);
    ser.serialize_map_end();
  }
};

template<>
struct SerializeT<std::multimap> {
  template<typename Key, typename Value, typename Cmp, typename Alloc>
  static void serialize(Serializer& ser, const std::multimap<Key, Value, Cmp, Alloc>& multimap) {
    ser.serialize_map_begin();
    for (auto& it : multimap)
      ser.serialize_map_entry(it.first, it.second);
    ser.serialize_map_end();
  }
};

} // namespace serde
