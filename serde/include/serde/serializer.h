#pragma once

#include "error.h"
#include "result.h"
#include "serialize.h"

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
  template<typename T>
  void serialize(T&& v) { serde::serialize(*this, v); }

  // Scalars ///////////////////////////////////////////////////////////////////
  virtual void serialize_bool(bool v) = 0;
  virtual void serialize_int(int v) = 0;
  virtual void serialize_uint(unsigned int v) = 0;
  virtual void serialize_lint(long int v) = 0;
  virtual void serialize_llint(long long int v) = 0;
  virtual void serialize_luint(unsigned long int v) = 0;
  virtual void serialize_lluint(unsigned long long int v) = 0;
  virtual void serialize_float(int v) = 0;
  virtual void serialize_double(int v) = 0;
  virtual void serialize_char(char v) = 0;
  virtual void serialize_cstr(const char* v) = 0;
  virtual void serialize_bytes(unsigned char* v, size_t len) = 0;

  // Optional //////////////////////////////////////////////////////////////////
  template<typename T>
  void serialize_some(T &&v) { serialize(v); }
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
  void serialize_map_key(K&& key) {
    serialize_map_key_begin();
    serialize(std::decay_t<K>(key));
    serialize_map_key_end();
  }

  template<typename V>
  void serialize_map_value(V&& value) {
    serialize_map_value_begin();
    serialize(value);
    serialize_map_value_end();
  }

  template<typename K, typename V>
  void serialize_map_entry(K&& key, V&& value) {
    serialize_map_key(key);
    serialize_map_value(value);
  }

  // Struct ////////////////////////////////////////////////////////////////////
  virtual void serialize_struct_begin() = 0;
  virtual void serialize_struct_end() = 0;
  virtual void serialize_struct_field_begin(const char* name) = 0;
  virtual void serialize_struct_field_end() = 0;

  template<typename V>
  void serialize_struct_field(const char* name, V&& value) {
    serialize_struct_field_begin(name);
    serialize(value);
    serialize_struct_field_end();
  }

  // Destructor
  virtual ~Serializer() = default;
};

} // namespace serde

