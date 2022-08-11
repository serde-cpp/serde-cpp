#pragma once

#include <set>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeT<std::set> {
  template<typename Key, typename Cmp, typename Alloc>
  static void serialize(Serializer& ser, const std::set<Key, Cmp, Alloc>& set) {
    ser.serialize_seq_begin();
    for (auto& e : set)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

template<>
struct SerializeT<std::multiset> {
  template<typename Key, typename Cmp, typename Alloc>
  static void serialize(Serializer& ser, const std::multiset<Key, Cmp, Alloc>& multiset) {
    ser.serialize_seq_begin();
    for (auto& e : multiset)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

} // namespace serde
