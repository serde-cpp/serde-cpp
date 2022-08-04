#include <gtest/gtest.h>

#include "serde/std.h"
#include "serde/serde.h"
#include "serde_yaml/serde_yaml.h"

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// std::string
///////////////////////////////////////////////////////////////////////////////

TEST(StdString, Simple)
{
  using Type = std::string;
  const Type val = "Hello World";
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "Hello World\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdString, Empty)
{
  using Type = std::string;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::string_view
///////////////////////////////////////////////////////////////////////////////

TEST(StdStringView, Simple)
{
  using Type = std::string_view;
  const Type val = "Hello World";
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "Hello World\n");
  // no deserialization for string_view
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Deserialize<std::basic_string_view>::deserialize<char, std::char_traits<char>>)>);
}

TEST(StdStringView, Empty)
{
  using Type = std::string_view;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "\n");
  // no deserialization for string_view
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Deserialize<std::basic_string_view>::deserialize<char, std::char_traits<char>>)>);
}

///////////////////////////////////////////////////////////////////////////////
// std::unique_ptr
///////////////////////////////////////////////////////////////////////////////

TEST(StdUniquePtr, Simple)
{
  using Type = std::unique_ptr<std::string>;
  const Type val = std::make_unique<std::string>("Potatos");
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "Potatos\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(*de_val, *val);
}

TEST(StdUniquePtr, Empty)
{
  using Type = std::unique_ptr<std::string>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "null\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::shared_ptr
///////////////////////////////////////////////////////////////////////////////

TEST(StdSharedPtr, Simple)
{
  using Type = std::shared_ptr<std::string>;
  const Type val = std::make_shared<std::string>("Bananas");
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "Bananas\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(*de_val, *val);
}

TEST(StdSharedPtr, Empty)
{
  using Type = std::shared_ptr<std::string>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "null\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::optional
///////////////////////////////////////////////////////////////////////////////

TEST(StdOptional, Value)
{
  using Type = std::optional<types::Number>;
  const Type val = types::Number::Three;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "Three\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ((int)*val, (int)*de_val);
}

TEST(StdOptional, Null)
{
  using Type = std::optional<types::Number>;
  const Type val = std::nullopt;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "null\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_FALSE(de_val.has_value());
}

///////////////////////////////////////////////////////////////////////////////
// std::array
///////////////////////////////////////////////////////////////////////////////

TEST(StdArray, Simple)
{
  using Type = std::array<size_t, 6>;
  const Type val = {56, 333, 1, 3, 49, 100};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- 56\n- 333\n- 1\n- 3\n- 49\n- 100\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdArray, Empty)
{
  using Type = std::array<size_t, 0>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::vector
///////////////////////////////////////////////////////////////////////////////

TEST(StdVector, Simple)
{
  using Type = std::vector<size_t>;
  const Type val = {56, 333, 1, 3, 49, 100};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- 56\n- 333\n- 1\n- 3\n- 49\n- 100\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdVector, Empty)
{
  using Type = std::vector<size_t>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::variant
///////////////////////////////////////////////////////////////////////////////

TEST(StdVariant, Index0) {
  using Type = std::variant<char, int, std::string>;
  const Type val = 'c';
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "0: c\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdVariant, Index1) {
  using Type = std::variant<char, int, std::string>;
  const Type val = 431;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "1: 431\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdVariant, Index2) {
  using Type = std::variant<char, int, std::string>;
  const Type val = "Hello World";
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "2: Hello World\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::tuple
///////////////////////////////////////////////////////////////////////////////

TEST(StdTuple, Simple)
{
  using Type = std::tuple<char, int, std::string>;
  const Type val = {'z', 3467, "MyTuple"};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- z\n- 3467\n- MyTuple\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::pair
///////////////////////////////////////////////////////////////////////////////

TEST(StdPair, Simple)
{
  using Type = std::pair<int, std::string>;
  const Type val = {69, "sixty-nine"};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "first: 69\nsecond: 'sixty-nine'\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::initializer_list
///////////////////////////////////////////////////////////////////////////////

TEST(StdInitializerList, Simple)
{
  using Type = std::initializer_list<std::string>;
  const Type val = {"apple", "banana", "orange", "avocado", "blueberry"};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- apple\n- banana\n- orange\n- avocado\n- blueberry\n");
  // no deserialization for initializer_list
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Deserialize<std::initializer_list>::deserialize<std::string>)>);
}

TEST(StdInitializerList, Empty)
{
  using Type = std::initializer_list<std::string>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " []\n");
  // no deserialization for initializer_list
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Deserialize<std::initializer_list>::deserialize<std::string>)>);
}

///////////////////////////////////////////////////////////////////////////////
// std::list
///////////////////////////////////////////////////////////////////////////////

TEST(StdList, Simple)
{
  using Type = std::list<int>;
  const Type val = {7, 9, 4, -1};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- 7\n- 9\n- 4\n- -1\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdList, Empty)
{
  using Type = std::list<int>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::forward_list
///////////////////////////////////////////////////////////////////////////////

TEST(StdForwardList, Simple)
{
  using Type = std::forward_list<int>;
  const Type val = {7, 9, 4, -1};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- 7\n- 9\n- 4\n- -1\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdForwardList, Empty)
{
  using Type = std::forward_list<int>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::set
///////////////////////////////////////////////////////////////////////////////

TEST(StdSet, Simple)
{
  using Type = std::set<char>;
  const Type val = {'w', 'o', 'r', 'l', 'd'};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- d\n- l\n- o\n- r\n- w\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdSet, Empty)
{
  using Type = std::set<char>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::unordered_set
///////////////////////////////////////////////////////////////////////////////

TEST(StdUnorderedSet, Simple)
{
  using Type = std::unordered_set<char>;
  const Type val = {'w', 'o', 'r', 'l', 'd'};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- d\n- l\n- r\n- o\n- w\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdUnorderedSet, Empty)
{
  using Type = std::unordered_set<char>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::multiset
///////////////////////////////////////////////////////////////////////////////

TEST(StdMultiSet, Simple)
{
  using Type = std::multiset<char>;
  const Type val = {'b', 'e', 'e', 'f'};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- b\n- e\n- e\n- f\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdMultiSet, Empty)
{
  using Type = std::multiset<char>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::unordered_multiset
///////////////////////////////////////////////////////////////////////////////

TEST(StdUnorderedMultiSet, Simple)
{
  using Type = std::unordered_multiset<char>;
  const Type val = {'b', 'e', 'e', 'f'};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- f\n- e\n- e\n- b\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdUnorderedMultiSet, Empty)
{
  using Type = std::unordered_multiset<char>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::map
///////////////////////////////////////////////////////////////////////////////

TEST(StdMap, Simple)
{
  using Type = std::map<std::string, long int>;
  const Type val = {{"foo", 10}, {"bar", 22}, {"egg", 67}};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "bar: 22\negg: 67\nfoo: 10\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdMap, Empty)
{
  using Type = std::map<std::string, long int>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " {}\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::unordered_map
///////////////////////////////////////////////////////////////////////////////

TEST(StdUnorderedMap, Simple)
{
  using Type = std::unordered_map<std::string, long int>;
  const Type val = {{"foo", 10}, {"bar", 22}, {"egg", 67}};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "egg: 67\nbar: 22\nfoo: 10\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdUnorderedMap, Empty)
{
  using Type = std::unordered_map<std::string, long int>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " {}\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::multimap
///////////////////////////////////////////////////////////////////////////////

TEST(StdMultiMap, Simple)
{
  using Type = std::multimap<short int, std::string>;
  const Type val = {{1, "one"}, {1, "uno"}, {2, "two"}};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "1: one\n1: uno\n2: two\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdMultiMap, Empty)
{
  using Type = std::multimap<std::string, long int>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " {}\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::unordered_multimap
///////////////////////////////////////////////////////////////////////////////

TEST(StdUnorderedMultiMap, Simple)
{
  using Type = std::unordered_multimap<short int, std::string>;
  const Type val = {{1, "one"}, {1, "uno"}, {2, "two"}};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "2: two\n1: uno\n1: one\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdUnorderedMultiMap, Empty)
{
  using Type = std::unordered_multimap<std::string, long int>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " {}\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::deque
///////////////////////////////////////////////////////////////////////////////

TEST(StdDeque, Simple)
{
  using Type = std::deque<std::string>;
  const Type val = {"clubs", "queen", "king"};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- clubs\n- queen\n- king\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdDeque, Empty)
{
  using Type = std::deque<std::string>;
  const Type val = {};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), " []\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::queue
///////////////////////////////////////////////////////////////////////////////

TEST(StdQueue, Simple)
{
  using Type = std::queue<std::string>;
  Type val; val.push("clubs"); val.push("queen"); val.push("king");
  std::string str = "- clubs\n- queen\n- king\n";
  // no serialization for queue
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Serialize<std::queue>::serialize<std::string>)>);
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdQueue, Empty)
{
  using Type = std::queue<std::string>;
  Type val;
  std::string str = " []\n";
  // no serialization for queue
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Serialize<std::queue>::serialize<std::string>)>);
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::stack
///////////////////////////////////////////////////////////////////////////////

TEST(StdStack, Simple)
{
  using Type = std::stack<std::string>;
  Type val; val.push("clubs"); val.push("queen"); val.push("king");
  std::string str = "- clubs\n- queen\n- king\n";
  // no serialization for stack
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Serialize<std::stack>::serialize<std::string>)>);
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(StdStack, Empty)
{
  using Type = std::stack<std::string>;
  Type val;
  std::string str = " []\n";
  // no serialization for stack
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Serialize<std::stack>::serialize<std::string>)>);
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

