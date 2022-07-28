#include <memory>

#include "serde/serde.h"
#include "serde/std.h"

#include "types.h"

template<>
void serde::serialize(serde::Serializer& ser, const types::Number& number)
{
  const char* cstr = "<null>";
  switch (number) {
    case types::Number::One: cstr = "One"; break;
    case types::Number::Two: cstr = "Two"; break;
    case types::Number::Three: cstr = "Three"; break;
  }
  ser.serialize(cstr);
}

template<>
void serde::deserialize(serde::Deserializer& de, types::Number& number)
{
  char cstr[32] = {0};
  de.deserialize(cstr);
  std::string_view str(cstr);
  if (str == "One") number = types::Number::One;
  else if (str == "Two") number = types::Number::Two;
  else if (str == "Three") number = types::Number::Three;
  else return; // TODO: mark error
}

template<>
void serde::serialize(serde::Serializer& ser, const types::Point& point)
{
  ser.serialize_seq_begin();

    ser.serialize_seq_begin();
      ser.serialize_i32(point.x);
      ser.serialize_i32(point.y);
    ser.serialize_seq_end();
    ser.serialize_seq_begin();
      ser.serialize_seq_begin();
        ser.serialize_i32(point.x);
        ser.serialize_i32(point.y);
      ser.serialize_seq_end();
      ser.serialize_seq_begin();
        ser.serialize_i32(point.x);
        ser.serialize_i32(point.y);
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

  std::vector<types::Number> vec = {types::Number::One,
                                     types::Number::Two,
                                     types::Number::Three};
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

template<>
void serde::deserialize(serde::Deserializer& de, types::Point& point)
{
  //de.deserialize_seq_begin();
    //de.deserialize(point.x);
    //de.deserialize(point.y);
    //de.deserialize(point.num);
  //de.deserialize_seq_end();

  //char key;
  //de.deserialize_map_begin();
    //de.deserialize_map_entry(key, point.x);
    //de.deserialize_map_entry(key, point.y);
    //de.deserialize_map_entry(key, point.num);
  //de.deserialize_map_end();

  //de.deserialize_map_begin();
    //de.deserialize_map_entry_find("x", point.x);
    //de.deserialize_map_entry_find("y", point.y);
    //de.deserialize_map_entry_find("num", point.num);
  //de.deserialize_map_end();

  de.deserialize_struct_begin();
    de.deserialize_struct_field("x", point.x);
    de.deserialize_struct_field("y", point.y);
    de.deserialize_struct_field("num", point.num);
  de.deserialize_struct_end();
}

