#include <gtest/gtest.h>

#include "serde.h"
#include "serde_yaml.h"
#include "serde_rapidyaml.h"

namespace types {

struct Point {
  int x;
  int y;
};

} // namespace point

TEST(Main, Test)
{
  types::Point point;
  point.x = 10; point.y = 20;
  std::string str = serde_yaml::to_string(point).unwrap();
  std::cout << str << std::endl;
  EXPECT_STREQ(str.c_str(), R"(---
- - 10
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

TEST(Main, Test2)
{
  using types::Point;
  Point point = serde_yaml::from_str<Point>("---\n- abc\n- b\n- 10\n").unwrap();
}

TEST(Main, RapidYaml)
{
  types::Point point;
  point.x = 10; point.y = 20;
  std::string str = serde_rapidyaml::to_string(point).unwrap();
  std::cout << str << std::endl;
}
