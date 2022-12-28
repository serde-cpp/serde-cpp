#include <gtest/gtest.h>

#include "serde/std.h"
#include "serde/serde.h"
#include "serde_yaml/serde_yaml.h"

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Advanced tests
///////////////////////////////////////////////////////////////////////////////

TEST(Advanced, PointSerialize)
{
  types::Point point{ 10, 20 };
  std::string str = serde_yaml::to_string(point).value();
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


TEST(Advanced, PointDeserialize)
{
  using ::types::Point;
  Point point = serde_yaml::from_str<Point>("{x: 0x10, y: 0x20, num: Three}").value();
  std::cout << "point: " << point.x << " " << point.y << " " << (int)point.num << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
// Local & Private scope types
///////////////////////////////////////////////////////////////////////////////

TEST(Advanced, LocalPrivateTypes)
{
  struct Local {
    Local() : line(0), scope{false} {}
    Local(int line) : line(line), scope{line != 0} {}
    void serialize(serde::Serializer& ser) const {
      ser.serialize_struct_begin();
      ser.serialize_struct_field("line", line);
      ser.serialize_struct_field("scope", scope);
      ser.serialize_struct_end();
    }
    void deserialize(serde::Deserializer& de) {
      de.deserialize_struct_begin();
      de.deserialize_struct_field("line", line);
      de.deserialize_struct_field("scope", scope);
      de.deserialize_struct_end();
    }
    bool operator==(const Local& o) const { return std::tie(line, scope) == std::tie(o.line, o.scope); }
   private:
    struct Scope {
      bool closed;
      void serialize(serde::Serializer& ser) const { ser.serialize(closed); }
      void deserialize(serde::Deserializer& de) { de.deserialize(closed); }
      bool operator==(const Scope& o) const { return closed == o.closed; }
    };
    int line;
    Scope scope;
  };

  Local local{100};

  auto str = serde_yaml::to_string(local).value();
  std::cout << str << std::endl;
  EXPECT_STREQ(str.c_str(), "line: 100\nscope: 1\n");

  auto de_local = serde_yaml::from_str<Local>(std::move(str)).value();
  EXPECT_EQ(local, de_local);
}

///////////////////////////////////////////////////////////////////////////////
// De/Serialize specialization for incomplete Template type
///////////////////////////////////////////////////////////////////////////////

// Type forward-declaration
template<typename T> struct Foo;

namespace serde {
// Serialize specialization
template<>
struct SerializeT<Foo> {
  template<typename... U>
  static void serialize(Serializer& ser, const Foo<U...>& val) {
    ser.serialize(val.v);
  }
};
// Deserialize specialization
template<>
struct Deserialize<Foo> {
  template<typename... U>
  static void deserialize(Deserializer& de, Foo<U...>& val) {
    de.deserialize(val.v);
  }
};
}

// Type definition
template<typename T> struct Foo
{
  T v = 3;
};

TEST(Advanced, Foo)
{
  Foo<int> foo{};
  auto str = serde_yaml::to_string(foo).value();
  EXPECT_STREQ(str.c_str(), "3\n");
  auto de = serde_yaml::from_str<Foo<int>>(std::move(str)).value();
  EXPECT_EQ(foo.v, de.v);
}


///////////////////////////////////////////////////////////////////////////////
// De/Serialize specialization for incomplete type (struct/class)
///////////////////////////////////////////////////////////////////////////////

// Type forward-declaration
struct Bar;

namespace serde {
// Serialize specialization
template<typename T>
struct Serialize<T, std::enable_if_t<std::is_same_v<T, Bar>>> {
  static void serialize(Serializer& ser, const T& val) {
    ser.serialize(val.v);
  }
};
// Deserialize specialization
template<typename T>
struct DeserializeT<T, std::enable_if_t<std::is_same_v<T, Bar>>> {
  static void deserialize(Deserializer& de, T& val) {
    de.deserialize(val.v);
  }
};
} // namespace serde

// Type definition
struct Bar {
  int v = 5;
};

TEST(Advanced, Bar)
{
  Bar bar{};
  auto str = serde_yaml::to_string(bar).value();
  EXPECT_STREQ(str.c_str(), "5\n");
  auto de = serde_yaml::from_str<Bar>(std::move(str)).value();
  EXPECT_EQ(bar.v, de.v);
}

///////////////////////////////////////////////////////////////////////////////
// Serialize specialization for incomplete type (enum)
///////////////////////////////////////////////////////////////////////////////

// Type forward-declaration
enum class Egg;

namespace serde {
// Serialize specialization
template<typename T>
struct Serialize<T, std::enable_if_t<std::is_same_v<T, Egg>>> {
  static void serialize(Serializer& ser, const T& val) {
    const char* cstr = "<null>";
    switch (val) {
      case T::Yolk: cstr = "Yolk"; break;
      case T::Whites: cstr = "Whites"; break;
    }
    ser.serialize(cstr);
  }
};
// Deserialize specialization
template<typename T>
struct DeserializeT<T, std::enable_if_t<std::is_same_v<T, Egg>>> {
  static void deserialize(Deserializer& de, T& val) {
    char cstr[32] = {0};
    de.deserialize(cstr);
    std::string_view str(cstr);
    if (str == "Yolk") val = T::Yolk;
    else if (str == "Whites") val = T::Whites;
    else return; // TODO: mark error
  }
};
} // namespace serde

// Type definition
enum class Egg {
  Yolk,
  Whites,
};

TEST(Advanced, Egg)
{
  auto egg = Egg::Whites;
  auto str = serde_yaml::to_string(egg).value();
  EXPECT_STREQ(str.c_str(), "Whites\n");
  auto de = serde_yaml::from_str<Egg>(std::move(str)).value();
  EXPECT_EQ(egg, de);
}

