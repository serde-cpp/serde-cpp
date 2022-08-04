#include <gtest/gtest.h>

#include "serde/serde.h"
#include "serde_yaml/serde_yaml.h"
#include "serde/std.h"

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

