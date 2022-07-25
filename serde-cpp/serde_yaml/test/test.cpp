#include <gtest/gtest.h>

#include "serde/serde.h"
#include "serde_yaml/serde_yaml.h"
#include "serde/std.h"

#include "types.h"

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
  Point point = serde_yaml::from_str<Point>("{x: 0x10, y: 0x20, num: Three}").unwrap();
  std::cout << "point: " << point.x << " " << point.y << " " << (int)point.num << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
// std::variant
///////////////////////////////////////////////////////////////////////////////

TEST(Variant, TestOne)
{
  using Variant = std::variant<char, int, std::string>;

  Variant variant = "Hello World";
  auto str = serde_yaml::to_string(variant).unwrap();
  std::cout << str << std::endl;
  EXPECT_STREQ(str.c_str(), "2: Hello World\n");

  Variant de_var = serde_yaml::from_str<Variant>(std::move(str)).unwrap();
  std::cout << de_var.index() << ": " << std::get<std::string>(de_var) << std::endl;
  EXPECT_EQ(variant, de_var);
}

///////////////////////////////////////////////////////////////////////////////
// std::tuple
///////////////////////////////////////////////////////////////////////////////

TEST(Tuple, TestOne)
{
  using Tuple = std::tuple<char, int, std::string>;

  Tuple tuple = {'z', 3467, "MyTuple"};
  auto str = serde_yaml::to_string(tuple).unwrap();
  std::cout << str << std::endl;
  EXPECT_STREQ(str.c_str(), "- z\n- 3467\n- MyTuple\n");

  Tuple de_tuple = serde_yaml::from_str<Tuple>(std::move(str)).unwrap();
  std::cout << "- " << std::get<0>(de_tuple) << '\n'
            << "- " << std::get<1>(de_tuple) << '\n'
            << "- " << std::get<2>(de_tuple) << '\n';
  EXPECT_EQ(tuple, de_tuple);
}

