#pragma once

#include <queue>
#include "../deserialize.h"
#include "../deserializer.h"

namespace serde {

template<>
struct DeserializeT<std::queue> {
  template<typename T, typename Seq>
  static void deserialize(Deserializer& de, std::queue<T, Seq>& queue) {
    while (!queue.empty()) queue.pop(); // clear queue
    size_t size = 0;
    de.deserialize_seq_size(size);
    de.deserialize_seq_begin();
    for (size_t i = 0; i < size; i++) {
      T val;
      de.deserialize(val);
      queue.push(std::move(val));
    }
    de.deserialize_seq_end();
  }
};

template<>
struct DeserializeT<std::priority_queue> {
  template<typename T, typename Seq, typename Cmp>
  static void deserialize(Deserializer& de, std::priority_queue<T, Seq, Cmp>& queue) {
    while (!queue.empty()) queue.pop(); // clear queue
    size_t size = 0;
    de.deserialize_seq_size(size);
    de.deserialize_seq_begin();
    for (size_t i = 0; i < size; i++) {
      T val;
      de.deserialize(val);
      queue.push(std::move(val));
    }
    de.deserialize_seq_end();
  }
};


} // namespace serde

