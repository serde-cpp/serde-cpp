#pragma once

#include <vector>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::vector> {
  template<typename... U>
  static void serialize(Serializer& ser, const std::vector<U...>& vec) {
    ser.serialize_seq_begin();
    for (auto& e : vec)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

} // namespace serde

