#include "result.h"

template<typename T, typename S, typename E>
struct Serialize {
  static auto serialize(S& ser, T& obj) -> Result<void, E>;
};

template<typename T, typename D, typename E>
struct Deserialize {
  static auto deserialize(D& de) -> Result<T, E>;
};


template<typename K, typename E>
struct SerializeSeq {
  using Ok = K;
  using Error = E;
  template<typename T>
  auto serialize_element(T&& v) -> Result<Ok, Error>;
  auto end_seq() -> Result<Ok, Error>;
};

template<typename K, typename E>
struct Serializer {
  using Ok = K;
  using Error = E;
  auto serialize_bool(bool v) -> Result<Ok, Error>;
  auto serialize_int(int v) -> Result<Ok, Error>;
  auto serialize_uint(unsigned int v) -> Result<Ok, Error>;
  auto serialize_lint(long int v) -> Result<Ok, Error>;
  auto serialize_llint(long long int v) -> Result<Ok, Error>;
  auto serialize_luint(unsigned long int v) -> Result<Ok, Error>;
  auto serialize_lluint(unsigned long long int v) -> Result<Ok, Error>;
  auto serialize_char(char v) -> Result<Ok, Error>;
  auto serialize_cstr(char* v) -> Result<Ok, Error>;
  auto serialize_bytes(unsigned char* v, size_t len) -> Result<Ok, Error>;
  template<typename T>
  auto serialize_some(T&& v) -> Result<Ok, Error>;
  auto serialize_none() -> Result<Ok, Error>;
  auto serialize_tuple() -> Result<Ok, Error>;
  template<typename It>
  auto serialize_seq(It&& begin, It&& end) -> Result<Ok, Error>;
  auto serialize_map() -> Result<Ok, Error>;
};


struct Deserializer {
};
