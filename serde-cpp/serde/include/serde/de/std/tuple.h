#pragma once

#include <tuple>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::tuple> {
  template<typename... Ts>
  static void deserialize(Deserializer& de, std::tuple<Ts...>& tuple) {
    de.deserialize_seq_begin();
    std::apply([&de] (auto&... args) {
      (de.deserialize(args), ...);
    }, tuple);
    de.deserialize_seq_end();
  }
};

} // namespace serde

