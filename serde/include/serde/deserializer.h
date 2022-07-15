#pragma once

#include "result.h"
#include "deserialize.h"

namespace serde {


class Deserializer {
public:
  using Result = ::Result<void, Error>;
  template<typename T> Result deserialize(std::decay_t<T>& v) {
    serde::deserialize(*this, v);
  }

  //virtual Result deserialize_bool(bool& v) = 0;
  //virtual Result deserialize_int(int& v) = 0;
  //virtual Result deserialize_cstr(char*& v, size_t n) = 0;
};

} // namespace serde

