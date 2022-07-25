#pragma once

namespace types {

enum class Number {
  One,
  Two,
  Three,
};

struct Point {
  int x;
  int y;
  Number num;
};

} // namespace types

