#pragma once

#include <initializer_list>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeT<std::initializer_list> {
  template<typename T>
  static void serialize(Serializer& ser, const std::initializer_list<T>& list) {
    ser.serialize_seq_begin();
    for (auto& e : list)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

} // namespace serde
