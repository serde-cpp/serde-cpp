#pragma once

#include "result.h"

namespace serde {

template<typename T>
void serialize(class Serializer& ser, const T& v);

class Serializer {
public:
  //virtual void serialize_bool(bool v) = 0;
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

  template<typename T> void serialize_some(T &&v) {
    serialize(*this, v);
  }
  // virtual void serialize_none() = 0;
  //virtual void serialize_tuple() = 0;

  virtual void serialize_seq_beg() = 0;
  virtual void serialize_seq_end() = 0;

  virtual void serialize_map_beg() = 0;
  virtual void serialize_map_end() = 0;

  virtual void serialize_map_key_beg() = 0;
  virtual void serialize_map_key_end() = 0;
  virtual void serialize_map_value_beg() = 0;
  virtual void serialize_map_value_end() = 0;

public:
  template<typename K>
  void serialize_map_key(K&& key) {
    serialize_map_key_beg();
    serde::serialize(*this, std::forward<std::decay_t<K>>(key));
    serialize_map_key_end();
  }

  template<typename V>
  void serialize_map_value(V&& value) {
    serialize_map_value_beg();
    serde::serialize(*this, value);
    serialize_map_value_end();
  }
  template<typename K, typename V>
  void serialize_map_entry(K&& key, V&& value) {
    serialize_map_key(key);
    serialize_map_value(value);
  }


  virtual ~Serializer() = default;
};


struct Deserializer {
};

} // namespace serde

//template<typename D, typename T>
//auto deserialize(Deserializer<D>& de) -> typename D::Result;
