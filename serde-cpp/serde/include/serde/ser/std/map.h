#pragma once

#include <map>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::map> {
  template<typename Key, typename Value, typename... U>
  static void serialize(Serializer& ser, const std::map<Key, Value, U...>& map) {
    ser.serialize_map_begin();
    for (auto& it : map)
      ser.serialize_map_entry(it.first, it.second);
    ser.serialize_map_end();
  }
};

template<>
struct Serialize<std::multimap> {
  template<typename Key, typename Value, typename... U>
  static void serialize(Serializer& ser, const std::multimap<Key, Value, U...>& multimap) {
    ser.serialize_map_begin();
    for (auto& it : multimap)
      ser.serialize_map_entry(it.first, it.second);
    ser.serialize_map_end();
  }
};

} // namespace serde

