#pragma once

#include <vector>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeT<std::vector> {
  template<typename T, typename Alloc>
  static void serialize(Serializer& ser, const std::vector<T, Alloc>& vec) {
    ser.serialize_seq_begin();
    for (auto& e : vec)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

} // namespace serde
