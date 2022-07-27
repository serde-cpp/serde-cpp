#pragma once

#include <list>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::list> {
  template<typename... U>
  static void serialize(Serializer& ser, const std::list<U...>& list) {
    ser.serialize_seq_begin();
    for (auto& e : list)
      ser.serialize(e);
    ser.serialize_seq_end();
  }
};

} // namespace serde

