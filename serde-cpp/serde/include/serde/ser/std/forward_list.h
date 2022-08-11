#pragma once

#include <forward_list>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeT<std::forward_list> {
  template<typename T, typename Alloc>
  static void serialize(Serializer& ser, const std::forward_list<T, Alloc>& list) {
    ser.serialize_seq_begin();
    for (auto& e : list)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

} // namespace serde
