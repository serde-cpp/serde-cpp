#pragma once

#include <deque>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::deque> {
  template<typename... U>
  static void serialize(Serializer& ser, const std::deque<U...>& deque) {
    ser.serialize_seq_begin();
    for (auto& e : deque)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

} // namespace serde

