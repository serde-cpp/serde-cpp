#include "serde.h"
#include "serde_yaml.h"

namespace point {

struct Point {
  int x;
  int y;
};

template<typename S, typename K, typename E>
auto serialize(Point& point, serde::Serializer<S, K, E>& ser) -> typename S::Result
{
  if (auto ret = ser.serialize_int(point.x); ret.isErr())
    return ret;
  return ser.serialize_int(point.y);
}
}

int main()
{
  point::Point point = serde_yaml::from_str<point::Point>("{x: 1, y: 2}").unwrap();
  std::cout << point.x << std::endl;
  std::cout << point.y << std::endl;

  point.x = 10; point.y = 20;
  std::string str = serde_yaml::to_string(point).unwrap();
  std::cout << str << std::endl;

  return 0;
}
