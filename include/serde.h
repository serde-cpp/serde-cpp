#pragma once

#include "result.h"

namespace serde {

struct Serializer {
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
  //virtual void serialize_cstr(char* v) = 0;
  //virtual void serialize_bytes(unsigned char* v, size_t len) = 0;

  template<typename T> void serialize_some(T&& v) { serialize(*this, v); }
  //virtual void serialize_none() = 0;

  virtual void serialize_seq_beg() = 0;
  virtual void serialize_seq_end() = 0;

  //virtual void serialize_tuple() = 0;
  //virtual void serialize_map() = 0;

  virtual ~Serializer() = default;
};


struct Deserializer {
};


} // namespace serde

//auto serialize(T& obj, serde::Serializer<S, K, E>& ser) -> typename S::Result;

//template<typename D, typename T>
//auto deserialize(Deserializer<D>& de) -> typename D::Result;
