#include <gtest/gtest.h>

#include "serde/serde.h"
#include "serde_yaml/serde_yaml.h"
#include "serde/std.h"

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// std::variant
///////////////////////////////////////////////////////////////////////////////

TEST(Variant, Index0) {
  using Type = std::variant<char, int, std::string>;
  const Type val = 'c';
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "0: c\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

TEST(Variant, Index1) {
  using Type = std::variant<char, int, std::string>;
  const Type val = 431;
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "1: 431\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

TEST(Variant, Index2) {
  using Type = std::variant<char, int, std::string>;
  const Type val = "Hello World";
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "2: Hello World\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

///////////////////////////////////////////////////////////////////////////////
// std::tuple
///////////////////////////////////////////////////////////////////////////////

TEST(Tuple, Simple)
{
  using Type = std::tuple<char, int, std::string>;
  const Type val = {'z', 3467, "MyTuple"};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "- z\n- 3467\n- MyTuple\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

///////////////////////////////////////////////////////////////////////////////
// std::optional
///////////////////////////////////////////////////////////////////////////////

TEST(Optional, Value)
{
  using Type = std::optional<types::Number>;
  const Type val = types::Number::Three;
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "Three\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ((int)*val, (int)*de_val);
}

TEST(Optional, Null)
{
  using Type = std::optional<types::Number>;
  const Type val = std::nullopt;
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "null\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_FALSE(de_val.has_value());
}

///////////////////////////////////////////////////////////////////////////////
// std::pair
///////////////////////////////////////////////////////////////////////////////

TEST(Pair, Simple)
{
  using Type = std::pair<int, std::string>;
  const Type val = {69, "sixty-nine"};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "first: 69\nsecond: 'sixty-nine'\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

///////////////////////////////////////////////////////////////////////////////
// std::initializer_list
///////////////////////////////////////////////////////////////////////////////

TEST(InitializerList, Simple)
{
  using Type = std::initializer_list<std::string>;
  const Type val = {"apple", "banana", "orange", "avocado", "blueberry"};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "- apple\n- banana\n- orange\n- avocado\n- blueberry\n");
  // no deserialization for initializer_list
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Deserialize<std::initializer_list>::deserialize<std::string>)>);
}

TEST(InitializerList, Empty)
{
  using Type = std::initializer_list<std::string>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), " []\n");
  // no deserialization for initializer_list
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Deserialize<std::initializer_list>::deserialize<std::string>)>);
}

///////////////////////////////////////////////////////////////////////////////
// std::set
///////////////////////////////////////////////////////////////////////////////

TEST(Set, Simple)
{
  using Type = std::set<char>;
  const Type val = {'w', 'o', 'r', 'l', 'd'};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "- d\n- l\n- o\n- r\n- w\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

TEST(Set, Empty)
{
  using Type = std::set<char>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

///////////////////////////////////////////////////////////////////////////////
// std::map
///////////////////////////////////////////////////////////////////////////////

TEST(Map, Simple)
{
  using Type = std::map<std::string, long int>;
  const Type val = {{"foo", 10}, {"bar", 22}, {"egg", 67}};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "bar: 22\negg: 67\nfoo: 10\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

TEST(Map, Empty)
{
  using Type = std::map<std::string, long int>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), " {}\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

///////////////////////////////////////////////////////////////////////////////
// std::array
///////////////////////////////////////////////////////////////////////////////

TEST(Array, Simple)
{
  using Type = std::array<size_t, 6>;
  const Type val = {56, 333, 1, 3, 49, 100};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "- 56\n- 333\n- 1\n- 3\n- 49\n- 100\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

TEST(Array, Empty)
{
  using Type = std::array<size_t, 0>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

///////////////////////////////////////////////////////////////////////////////
// std::vector
///////////////////////////////////////////////////////////////////////////////

TEST(Vector, Simple)
{
  using Type = std::vector<size_t>;
  const Type val = {56, 333, 1, 3, 49, 100};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "- 56\n- 333\n- 1\n- 3\n- 49\n- 100\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

TEST(Vector, Empty)
{
  using Type = std::vector<size_t>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

