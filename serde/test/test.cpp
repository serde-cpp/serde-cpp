#include <gtest/gtest.h>

#include "serde.h"
#include "serde_yaml.h"

namespace types {

struct Point {
  int x;
  int y;
};

} // namespace point

TEST(Main, Test)
{
  types::Point point = serde_yaml::from_str<types::Point>("{x: 1, y: 2}").unwrap();
  std::cout << point.x << std::endl;
  std::cout << point.y << std::endl;

  point.x = 10; point.y = 20;
  std::string str = serde_yaml::to_string(point).unwrap();
  std::cout << str << std::endl;
}

