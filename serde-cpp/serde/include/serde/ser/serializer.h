#pragma once

#include "../error.h"
#include "../result.h"
#include "serialize.h"
#include "traits.h"

namespace serde {

////////////////////////////////////////////////////////////////////////////////
/// Serializer interface
///
/// Datatypes should implement serde::serialize function and call the serializer
/// methods to serialize primitive types, sequences, maps, structs, etc.
///
/// Dataformats should implement the methods in this interface to serialize
/// datatypes to its corresponding dataformat, e.g. yaml, json, toml, etc.
/// Conventionally Serializers should provide to_string() and from_str()
/// functions for serializing any datatype to string and deserializing from
/// string to any datatype.
class Serializer {
public:
  // serde::serialize should be implemented (specialized) for user types.
  // most std types have builtin implementation from the library
  template<typename T>
  inline void serialize(const T& v) {
    if constexpr (traits::HasMemberSerialize<T>::value) v.serialize(*this);
    else serde::serialize(*this, v);
  }

  // Scalars ///////////////////////////////////////////////////////////////////
  // note: prefer using serde::serialize() for standard types
  // int, unsigned int, short int, long int, size_t, intptr_t ...
  // as they will get resolved to corresponding fixed width integer types by
  // the compiler and serde::serialize() is implemented for those scalars.
  virtual void serialize_bool(bool) = 0;
  virtual void serialize_i8(int8_t) = 0;
  virtual void serialize_u8(uint8_t) = 0;
  virtual void serialize_i16(int16_t) = 0;
  virtual void serialize_u16(uint16_t) = 0;
  virtual void serialize_i32(int32_t) = 0;
  virtual void serialize_u32(uint32_t) = 0;
  virtual void serialize_i64(int64_t) = 0;
  virtual void serialize_u64(uint64_t) = 0;
  virtual void serialize_float(float) = 0;
  virtual void serialize_double(double) = 0;
  virtual void serialize_char(char) = 0;
  virtual void serialize_uchar(unsigned char) = 0;
  virtual void serialize_cstr(const char*) = 0;
  virtual void serialize_bytes(unsigned char* val, size_t len) = 0;

  // Optional //////////////////////////////////////////////////////////////////
  virtual void serialize_none() = 0;

  // Sequence //////////////////////////////////////////////////////////////////
  virtual void serialize_seq_begin() = 0;
  virtual void serialize_seq_end() = 0;

  // Map ///////////////////////////////////////////////////////////////////////
  virtual void serialize_map_begin() = 0;
  virtual void serialize_map_end() = 0;
  virtual void serialize_map_key_begin() = 0;
  virtual void serialize_map_key_end() = 0;
  virtual void serialize_map_value_begin() = 0;
  virtual void serialize_map_value_end() = 0;

  template<typename K>
  inline void serialize_map_key(const K& key) {
    serialize_map_key_begin();
    serialize(key);
    serialize_map_key_end();
  }

  template<typename V>
  inline void serialize_map_value(const V& value) {
    serialize_map_value_begin();
    serialize(value);
    serialize_map_value_end();
  }

  template<typename K, typename V>
  inline void serialize_map_entry(const K& key, const V& value) {
    serialize_map_key(key);
    serialize_map_value(value);
  }

  // Struct ////////////////////////////////////////////////////////////////////
  virtual void serialize_struct_begin() = 0;
  virtual void serialize_struct_end() = 0;
  virtual void serialize_struct_field_begin(const char* name) = 0;
  virtual void serialize_struct_field_end() = 0;

  template<typename V>
  inline void serialize_struct_field(const char* name, const V& value) {
    serialize_struct_field_begin(name);
    serialize(value);
    serialize_struct_field_end();
  }

  // Destructor
  virtual ~Serializer() = default;
};

} // namespace serde

