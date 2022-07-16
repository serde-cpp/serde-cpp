#pragma once

#include "result.h"
#include "error.h"

namespace serde {

template<typename T>
Error deserialize(class Deserializer& ser, std::decay_t<T>& v);

} // namespace serde
