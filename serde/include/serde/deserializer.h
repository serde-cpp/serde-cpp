#pragma once

#include "result.h"
#include "deserialize.h"

namespace serde {

class Deserializer {
public:
  template<typename T>
  inline void deserialize(T& v) { serde::deserialize(*this, v); }

  // Scalars ///////////////////////////////////////////////////////////////////
  virtual void deserialize_bool(bool&) = 0;
  virtual void deserialize_i8(int8_t&) = 0;
  virtual void deserialize_u8(uint8_t&) = 0;
  virtual void deserialize_i16(int16_t&) = 0;
  virtual void deserialize_u16(uint16_t&) = 0;
  virtual void deserialize_i32(int32_t&) = 0;
  virtual void deserialize_u32(uint32_t&) = 0;
  virtual void deserialize_i64(int64_t&) = 0;
  virtual void deserialize_u64(uint64_t&) = 0;
  virtual void deserialize_float(float&) = 0;
  virtual void deserialize_double(double&) = 0;
  virtual void deserialize_char(char&) = 0;
  virtual void deserialize_uchar(unsigned char&) = 0;
  virtual void deserialize_cstr(char*, size_t len) = 0;
  virtual void deserialize_bytes(unsigned char* val, size_t len) = 0;

  // Optional //////////////////////////////////////////////////////////////////

  // Sequence //////////////////////////////////////////////////////////////////
  virtual void deserialize_seq_begin() = 0;
  virtual void deserialize_seq_count(size_t&) = 0;
  virtual void deserialize_seq_end() = 0;

  // Map ///////////////////////////////////////////////////////////////////////
  virtual void deserialize_map_begin() = 0;
  virtual void deserialize_map_end() = 0;
  virtual void deserialize_map_key_begin() = 0;
  virtual void deserialize_map_key_end() = 0;
  virtual void deserialize_map_key_find(const char* key) = 0;
  virtual void deserialize_map_value_begin() = 0;
  virtual void deserialize_map_value_end() = 0;

  template<typename K>
  inline void deserialize_map_key(K& key) {
    deserialize_map_key_begin();
    deserialize(key);
    deserialize_map_key_end();
  }

  template<typename V>
  inline void deserialize_map_value(V& value) {
    deserialize_map_value_begin();
    deserialize(value);
    deserialize_map_value_end();
  }

  template<typename K, typename V>
  inline void deserialize_map_entry(K& key, V& value) {
    deserialize_map_key(key);
    deserialize_map_value(value);
  }

  template<typename V>
  inline void deserialize_map_entry_find(const char* key, V& value) {
    deserialize_map_key_find(key);
    deserialize_map_value(value);
  }

  // Struct ////////////////////////////////////////////////////////////////////
  virtual void deserialize_struct_begin() = 0;
  virtual void deserialize_struct_end() = 0;
  virtual void deserialize_struct_field_begin(const char* name) = 0;
  virtual void deserialize_struct_field_end() = 0;

  template<typename V>
  inline void deserialize_struct_field(const char* name, V&& value) {
    deserialize_struct_field_begin(name);
    deserialize(value);
    deserialize_struct_field_end();
  }

  // Destructor
  virtual ~Deserializer() = default;
};

} // namespace serde

