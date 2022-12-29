#pragma once

#include <memory>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::unique_ptr> {
  template<typename T, typename Deleter>
  static void deserialize(Deserializer& de, std::unique_ptr<T, Deleter>& val) {
    bool is_some = false;
    de.deserialize_is_some(is_some);
    if (is_some) {
      if (!val) val.reset(new T());
      de.deserialize(*val);
    }
    else {
      val.reset();
      de.deserialize_none();
    }
  }
};

template<>
struct DeserializeT<std::shared_ptr> {
  template<typename T>
  static void deserialize(Deserializer& de, std::shared_ptr<T>& val) {
    bool is_some = false;
    de.deserialize_is_some(is_some);
    if (is_some) {
      if (!val) val.reset(new T());
      de.deserialize(*val);
    }
    else {
      val.reset();
      de.deserialize_none();
    }
  }
};

} // namespace serde

