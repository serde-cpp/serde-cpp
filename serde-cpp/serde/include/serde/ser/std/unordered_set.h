#pragma once

#include <unordered_set>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::unordered_set> {
  template<typename Key, typename... U>
  static void serialize(Serializer& ser, const std::unordered_set<Key, U...>& set) {
    ser.serialize_seq_begin();
    for (auto& e : set)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

template<>
struct Serialize<std::unordered_multiset> {
  template<typename Key, typename... U>
  static void serialize(Serializer& ser, const std::unordered_multiset<Key, U...>& multiset) {
    ser.serialize_seq_begin();
    for (auto& e : multiset)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

} // namespace serde



