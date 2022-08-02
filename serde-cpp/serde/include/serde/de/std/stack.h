#pragma once

#include <stack>

#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct Deserialize<std::stack> {
  template<typename T, typename... U>
  static void deserialize(Deserializer& de, std::stack<T, U...>& stack) {
    while (!stack.empty()) stack.pop(); // clear stack
    size_t size = 0;
    de.deserialize_seq_size(size);
    de.deserialize_seq_begin();
    for (size_t i = 0; i < size; i++) {
      T val;
      de.deserialize(val);
      stack.push(std::move(val));
    }
    de.deserialize_seq_end();
  }
};

} // namespace serde

