#include <gtest/gtest.h>

#include "serde/std.h"
#include "serde/serde.h"
#include "serde_yaml/serde_yaml.h"

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// std::string
///////////////////////////////////////////////////////////////////////////////

TEST(Std, String_Value)
{
  using Type = std::string;
  const Type val = "Hello World";
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "Hello World\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, String_Empty)
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

TEST(Std, StringView_Value)
{
  using Type = std::string_view;
  const Type val = "Hello World";
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "Hello World\n");
  // no deserialization for string_view
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Deserialize<std::basic_string_view>::deserialize<char, std::char_traits<char>>)>);
}

TEST(Std, StringView_Empty)
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

TEST(Std, UniquePtr_Value)
{
  using Type = std::unique_ptr<std::string>;
  const Type val = std::make_unique<std::string>("Potatoes");
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "Potatoes\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(*de_val, *val);
}

TEST(Std, UniquePtr_Empty)
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

TEST(Std, SharedPtr_Value)
{
  using Type = std::shared_ptr<std::string>;
  const Type val = std::make_shared<std::string>("Bananas");
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "Bananas\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(*de_val, *val);
}

TEST(Std, SharedPtr_Empty)
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

TEST(Std, Optional_Value)
{
  using Type = std::optional<int>;
  const Type val = 10;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "10\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ((int)*val, (int)*de_val);
}

TEST(Std, Optional_Null)
{
  using Type = std::optional<int>;
  const Type val = std::nullopt;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "null\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_FALSE(de_val.has_value());
}

///////////////////////////////////////////////////////////////////////////////
// std::array
///////////////////////////////////////////////////////////////////////////////

TEST(Std, Array_Value)
{
  using Type = std::array<size_t, 6>;
  const Type val = {56, 333, 1, 3, 49, 100};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- 56\n- 333\n- 1\n- 3\n- 49\n- 100\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, Array_Empty)
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

TEST(Std, Vector_Value)
{
  using Type = std::vector<size_t>;
  const Type val = {56, 333, 1, 3, 49, 100};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- 56\n- 333\n- 1\n- 3\n- 49\n- 100\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, Vector_Empty)
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

TEST(Std, Variant_Index0) {
  using Type = std::variant<char, int, std::string>;
  const Type val = 'c';
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "0: c\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, Variant_Index1) {
  using Type = std::variant<char, int, std::string>;
  const Type val = 431;
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "1: 431\n");
  Type de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, Variant_Index2) {
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

TEST(Std, Tuple_Value)
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

TEST(Std, Pair_Value)
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

TEST(Std, InitializerList_Value)
{
  using Type = std::initializer_list<std::string>;
  const Type val = {"apple", "banana", "orange", "avocado", "blueberry"};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- apple\n- banana\n- orange\n- avocado\n- blueberry\n");
  // no deserialization for initializer_list
  static_assert(!std::is_member_function_pointer_v<decltype(&serde::Deserialize<std::initializer_list>::deserialize<std::string>)>);
}

TEST(Std, InitializerList_Empty)
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

TEST(Std, List_Value)
{
  using Type = std::list<int>;
  const Type val = {7, 9, 4, -1};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- 7\n- 9\n- 4\n- -1\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, List_Empty)
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

TEST(Std, ForwardList_Value)
{
  using Type = std::forward_list<int>;
  const Type val = {7, 9, 4, -1};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- 7\n- 9\n- 4\n- -1\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, ForwardList_Empty)
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

TEST(Std, Set_Value)
{
  using Type = std::set<char>;
  const Type val = {'w', 'o', 'r', 'l', 'd'};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- d\n- l\n- o\n- r\n- w\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, Set_Empty)
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

TEST(Std, UnorderedSet_Value)
{
  using Type = std::unordered_set<char>;
  const Type val = {'w', 'o', 'r', 'l', 'd'};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- d\n- l\n- r\n- o\n- w\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, UnorderedSet_Empty)
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

TEST(Std, MultiSet_Value)
{
  using Type = std::multiset<char>;
  const Type val = {'b', 'e', 'e', 'f'};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- b\n- e\n- e\n- f\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, MultiSet_Empty)
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

TEST(Std, UnorderedMultiSet_Value)
{
  using Type = std::unordered_multiset<char>;
  const Type val = {'b', 'e', 'e', 'f'};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- f\n- e\n- e\n- b\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, UnorderedMultiSet_Empty)
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

TEST(Std, Map_Value)
{
  using Type = std::map<std::string, long int>;
  const Type val = {{"foo", 10}, {"bar", 22}, {"egg", 67}};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "bar: 22\negg: 67\nfoo: 10\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, Map_Empty)
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

TEST(Std, UnorderedMap_Value)
{
  using Type = std::unordered_map<std::string, long int>;
  const Type val = {{"foo", 10}, {"bar", 22}, {"egg", 67}};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "egg: 67\nbar: 22\nfoo: 10\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, UnorderedMap_Empty)
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

TEST(Std, MultiMap_Value)
{
  using Type = std::multimap<short int, std::string>;
  const Type val = {{1, "one"}, {1, "uno"}, {2, "two"}};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "1: one\n1: uno\n2: two\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, MultiMap_Empty)
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

TEST(Std, UnorderedMultiMap_Value)
{
  using Type = std::unordered_multimap<short int, std::string>;
  const Type val = {{1, "one"}, {1, "uno"}, {2, "two"}};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "2: two\n1: uno\n1: one\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, UnorderedMultiMap_Empty)
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

TEST(Std, Deque_Value)
{
  using Type = std::deque<std::string>;
  const Type val = {"clubs", "queen", "king"};
  auto str = serde_yaml::to_string(val).value();
  EXPECT_STREQ(str.c_str(), "- clubs\n- queen\n- king\n");
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, Deque_Empty)
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

TEST(Std, Queue_Value)
{
  using Type = std::queue<std::string>;
  Type val; val.push("clubs"); val.push("queen"); val.push("king");
  std::string str = "- clubs\n- queen\n- king\n";
  // no serialization for queue
  //static_assert(!std::is_member_function_pointer_v<decltype(&serde::SerializeT<std::queue>::serialize<std::string>)>);
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, Queue_Empty)
{
  using Type = std::queue<std::string>;
  Type val;
  std::string str = " []\n";
  // no serialization for queue
  //static_assert(!std::is_member_function_pointer_v<decltype(&serde::SerializeT<std::queue>::serialize<std::string>)>);
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

///////////////////////////////////////////////////////////////////////////////
// std::stack
///////////////////////////////////////////////////////////////////////////////

TEST(Std, Stack_Value)
{
  using Type = std::stack<std::string>;
  Type val; val.push("clubs"); val.push("queen"); val.push("king");
  std::string str = "- clubs\n- queen\n- king\n";
  // no serialization for stack
  //static_assert(!std::is_member_function_pointer_v<decltype(&serde::SerializeT<std::stack>::serialize<std::string>)>);
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

TEST(Std, Stack_Empty)
{
  using Type = std::stack<std::string>;
  Type val;
  std::string str = " []\n";
  // no serialization for stack
  //static_assert(!std::is_member_function_pointer_v<decltype(&serde::SerializeT<std::stack>::serialize<std::string>)>);
  auto de_val = serde_yaml::from_str<Type>(std::move(str)).value();
  EXPECT_EQ(de_val, val);
}

