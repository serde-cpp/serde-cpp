#pragma once

#include <tuple>

#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct Serialize<std::tuple> {
  template<typename... U>
  static void serialize(Serializer& ser, const std::tuple<U...>& tuple) {
    ser.serialize_seq_begin();
    std::apply([&ser] (auto&... args) {
      (ser.serialize(args), ...);
    }, tuple);
    ser.serialize_seq_end();
  }
};

} // namespace serde

