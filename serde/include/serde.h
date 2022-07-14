#pragma once

#include "result.h"

namespace serde {

template<typename T>
void serialize(class Serializer& ser, const T& v);

//template<template<typename> typename T, typename U>
//void serialize(class Serializer& ser, const T<U>& v);

template<template<typename...> typename T, typename... U>
void serialize(class Serializer& ser, const T<U...>& v);

enum class Style {
  Inline,
  Fold,
};

class Serializer {
public:
  template<typename T> void serialize(T&& v) {
    serde::serialize(*this, v);
  }

  virtual void serialize_bool(bool v) = 0;
  virtual void serialize_int(int v) = 0;
  //virtual void serialize_uint(unsigned int v) = 0;
  //virtual void serialize_lint(long int v) = 0;
  //virtual void serialize_llint(long long int v) = 0;
  //virtual void serialize_luint(unsigned long int v) = 0;
  //virtual void serialize_lluint(unsigned long long int v) = 0;
  //virtual void serialize_float(int v) = 0;
  //virtual void serialize_double(int v) = 0;
  //virtual void serialize_char(char v) = 0;
  virtual void serialize_cstr(const char* v) = 0;
  //virtual void serialize_bytes(unsigned char* v, size_t len) = 0;

  // Optional
  template<typename T> void serialize_some(T &&v) { serialize(v); }
  virtual void serialize_none() = 0;

  // Sequence
  virtual void serialize_seq_begin(Style style = Style::Fold) = 0;
  virtual void serialize_seq_end() = 0;

  // Map
  virtual void serialize_map_begin(Style style = Style::Fold) = 0;
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

  // Struct
  virtual void serialize_struct_begin(Style style = Style::Fold) = 0;
  virtual void serialize_struct_end() = 0;

  virtual void serialize_struct_field_begin(const char* name) = 0;
  virtual void serialize_struct_field_end() = 0;

  template<typename V>
  void serialize_struct_field(const char* name, V&& value) {
    serialize_struct_field_begin(name);
    serialize(value);
    serialize_struct_field_end();
  }

  virtual ~Serializer() = default;
};


template<>
inline void serialize(Serializer& ser, const char* const & cstr)
{
  ser.serialize_cstr(cstr);
}

template<>
inline void serialize(Serializer& ser, const int& v)
{
  ser.serialize_int(v);
}

template<template<typename...> typename T, typename... U>
inline void serialize(Serializer& ser, T<U...> const& vec)
{
  ser.serialize_seq_begin();
  for (auto& v : vec) ser.serialize(v);
  ser.serialize_seq_end();
}

// ==================================================================

struct Error final {
  enum class Kind {
    Invalid,
  };

  Kind kind;
  size_t line = 0;
  size_t column = 0;
  std::string text = "";
};

template<typename T>
Error deserialize(class Deserializer& ser, const T& v);

class Deserializer {
public:
  using Result = ::Result<void, Error>;
  template<typename T> Result deserialize(std::decay_t<T>& v) {
    serde::deserialize(*this, v);
  }

  //virtual Result deserialize_bool(bool& v) = 0;
  //virtual Result deserialize_int(int& v) = 0;
  //virtual Result deserialize_cstr(char*& v, size_t n) = 0;
};

} // namespace serde

