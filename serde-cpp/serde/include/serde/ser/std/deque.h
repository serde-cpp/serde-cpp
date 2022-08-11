#pragma once

#include <deque>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeT<std::deque> {
  template<typename T, typename Alloc>
  static void serialize(Serializer& ser, const std::deque<T, Alloc>& deque) {
    ser.serialize_seq_begin();
    for (auto& e : deque)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

} // namespace serde
