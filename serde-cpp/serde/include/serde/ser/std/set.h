#pragma once

#include <set>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::set> {
  template<typename Key, typename... U>
  static void serialize(Serializer& ser, const std::set<Key, U...>& set) {
    ser.serialize_seq_begin();
    for (auto& e : set)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

} // namespace serde


