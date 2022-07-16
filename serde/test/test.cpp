#include <gtest/gtest.h>

#include "serde/serde.h"
#include "serde_yaml/serde_yaml.h"

namespace types {

enum class Numbers {
  One,
  Two,
  Three,
};

struct Point {
  int x;
  int y;
  Numbers num;
};

} // namespace point

TEST(Main, Serialize)
{
  types::Point point;
  point.x = 10; point.y = 20;
  std::string str = serde_yaml::to_string(point).unwrap();
  std::cout << str << std::endl;
  EXPECT_STREQ(str.c_str(), R"(- - 10
  - 20
- - - 10
    - 20
  - - 10
    - 20
- x: 10
  y: 20
  z:
    - 10
    - 20
- r:
    - 10
    - 20
- t: 10
  s: 10
- - One
  - Two
  - Three
- w:
    a: 10
    b: 20
)");
}


TEST(Main, Deserialize)
{
  using ::types::Point;
  Point point = serde_yaml::from_str<Point>("[10,20,Three]").unwrap();
  std::cout << "point: " << point.x << " " << point.y << " " << (int)point.num << std::endl;
}
