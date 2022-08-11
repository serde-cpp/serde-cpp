#pragma once

#include <tuple>
#include "../serialize.h"
#include "../serializer.h"

namespace serde {

template<>
struct SerializeT<std::tuple> {
  template<typename... Ts>
  static void serialize(Serializer& ser, const std::tuple<Ts...>& tuple) {
    ser.serialize_seq_begin();
    std::apply([&ser] (auto&... args) {
      (ser.serialize(args), ...);
    }, tuple);
    ser.serialize_seq_end();
  }
};

} // namespace serde
