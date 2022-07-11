#include "serde.h"

namespace point {

struct Point {
  int x;
  int y;
};

} // namespace point

template<>
void serde::serialize(serde::Serializer& ser, const char* const & cstr)
{
  ser.serialize_cstr(cstr);
}

template<>
void serde::serialize(serde::Serializer& ser, const int& v)
{
  ser.serialize_int(v);
}

template<>
void serde::serialize(serde::Serializer& ser, const point::Point& point)
{
  ser.serialize_seq_beg();

    ser.serialize_seq_beg();
      ser.serialize_int(point.x);
      ser.serialize_int(point.y);
    ser.serialize_seq_end();
    ser.serialize_seq_beg();
      ser.serialize_seq_beg();
        ser.serialize_int(point.x);
        ser.serialize_int(point.y);
      ser.serialize_seq_end();
      ser.serialize_seq_beg();
        ser.serialize_int(point.x);
        ser.serialize_int(point.y);
      ser.serialize_seq_end();
    ser.serialize_seq_end();

  ser.serialize_map_beg();
    ser.serialize_map_entry("x", point.x);
    ser.serialize_map_entry("y", point.y);

    ser.serialize_map_key("z");
    ser.serialize_map_value_beg();
      ser.serialize_seq_beg();
        ser.serialize_int(point.x);
        ser.serialize_int(point.y);
      ser.serialize_seq_end();
    ser.serialize_map_value_end();
  ser.serialize_map_end();

  ser.serialize_seq_end();
}

