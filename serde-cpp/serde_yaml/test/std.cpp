#include <gtest/gtest.h>

#include "serde/serde.h"
#include "serde_yaml/serde_yaml.h"
#include "serde/std.h"

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// std::variant
///////////////////////////////////////////////////////////////////////////////

struct Variant : public ::testing::Test {
  using Type = std::variant<char, int, std::string>;
};

TEST_F(Variant, Index0) {
  const Type val = 'c';
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "0: c\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

TEST_F(Variant, Index1) {
  const Type val = 431;
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "1: 431\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

TEST_F(Variant, Index2) {
  const Type val = "Hello World";
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "2: Hello World\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

///////////////////////////////////////////////////////////////////////////////
// std::tuple
///////////////////////////////////////////////////////////////////////////////

TEST(Tuple, TestOne)
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

TEST(Pair, TestOne)
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

TEST(InitializerList, TestOne)
{
  using Type = std::initializer_list<std::string>;
  const Type val = {"apple", "banana", "orange", "avocado", "blueberry"};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "- apple\n- banana\n- orange\n- avocado\n- blueberry\n");
  // no deserialization for initializer_list
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Deserialize<std::initializer_list>::deserialize<std::string>)>);
}

///////////////////////////////////////////////////////////////////////////////
// std::set
///////////////////////////////////////////////////////////////////////////////

TEST(Set, TestOne)
{
  using Type = std::set<char>;
  const Type val = {'w', 'o', 'r', 'l', 'd'};
  auto str = serde_yaml::to_string(val).unwrap();
  EXPECT_STREQ(str.c_str(), "- d\n- l\n- o\n- r\n- w\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).unwrap();
  EXPECT_EQ(val, de_val);
}

