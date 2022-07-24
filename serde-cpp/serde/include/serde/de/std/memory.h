#pragma once

#include <memory>

#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct Deserialize<std::unique_ptr> {
  template<typename T>
  static void deserialize(Deserializer& de, std::unique_ptr<T>& val) {
    if (!val)
      val.reset(new T());
    de.deserialize(*val);
  }
};

template<>
struct Deserialize<std::shared_ptr> {
  template<typename T>
  static void deserialize(Deserializer& de, std::shared_ptr<T>& val) {
    if (!val)
      val.reset(new T());
    de.deserialize(*val);
  }
};

} // namespace serde

