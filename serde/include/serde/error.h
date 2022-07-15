#pragma once

#include <string>

namespace serde {

struct Error final {
  enum class Kind {
    Invalid,
  };

  Kind kind;
  size_t line = 0;
  size_t column = 0;
  std::string text = "";
};

} // namespace serde

