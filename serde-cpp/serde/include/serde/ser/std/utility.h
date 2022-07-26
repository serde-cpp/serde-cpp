#pragma once

#include <utility>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::pair> {
  template<typename T1, typename T2>
  static void serialize(Serializer& ser, const std::pair<T1, T2>& pair) {
    ser.serialize_struct_begin();
    ser.serialize_struct_field("first", pair.first);
    ser.serialize_struct_field("second", pair.second);
    ser.serialize_struct_end();
  }
};

} // namespace serde

