#pragma once

#include <list>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::list> {
  template<typename T, typename Alloc>
  static void deserialize(Deserializer& de, std::list<T, Alloc>& list) {
    size_t size = 0;
    de.deserialize_seq_size(size);
    de.deserialize_seq_begin();
    list.resize(size);
    for (auto& e : list)
      de.deserialize(e);
    de.deserialize_seq_end();
  }
};

} // namespace serde

