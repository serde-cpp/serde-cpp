#include "serde/serde.h"

namespace types {

enum class Numbers {
  One,
  Two,
  Three,
};

struct Point {
  int x;
  int y;
};

} // namespace types

template<>
void serde::serialize(serde::Serializer& ser, const types::Numbers& number)
{
  const char* cstr = "<null>";
  switch (number) {
    case types::Numbers::One: cstr = "One"; break;
    case types::Numbers::Two: cstr = "Two"; break;
    case types::Numbers::Three: cstr = "Three"; break;
  }
  ser.serialize_cstr(cstr);
}

template<>
void serde::serialize(serde::Serializer& ser, const types::Point& point)
{
  ser.serialize_seq_begin();

    ser.serialize_seq_begin();
      ser.serialize_int(point.x);
      ser.serialize_int(point.y);
    ser.serialize_seq_end();
    ser.serialize_seq_begin();
      ser.serialize_seq_begin();
        ser.serialize_int(point.x);
        ser.serialize_int(point.y);
      ser.serialize_seq_end();
      ser.serialize_seq_begin();
        ser.serialize_int(point.x);
        ser.serialize_int(point.y);
      ser.serialize_seq_end();
    ser.serialize_seq_end();

  ser.serialize_map_begin();
    ser.serialize_map_entry("x", point.x);
    ser.serialize_map_entry("y", point.y);

    ser.serialize_map_key("z");
    ser.serialize_map_value_begin();
      ser.serialize_seq_begin();
        ser.serialize(point.x);
        ser.serialize(point.y);
      ser.serialize_seq_end();
    ser.serialize_map_value_end();
  ser.serialize_map_end();

  ser.serialize_struct_begin();
    ser.serialize_struct_field_begin("r");
      ser.serialize_seq_begin();
        ser.serialize(point.x);
        ser.serialize(point.y);
      ser.serialize_seq_end();
    ser.serialize_struct_field_end();
  ser.serialize_struct_end();

  ser.serialize_struct_begin();
    ser.serialize_struct_field("t", point.x);
    ser.serialize_struct_field("s", point.x);
  ser.serialize_struct_end();

  std::vector<types::Numbers> vec = {types::Numbers::One,
                                     types::Numbers::Two,
                                     types::Numbers::Three};
  ser.serialize(vec);

  ser.serialize_struct_begin();
    ser.serialize_struct_field_begin("w");
      ser.serialize_map_begin();
        ser.serialize_map_entry("a", point.x);
        ser.serialize_map_entry("b", point.y);
      ser.serialize_map_end();
    ser.serialize_struct_field_end();
  ser.serialize_struct_end();

  ser.serialize_seq_end();
}

