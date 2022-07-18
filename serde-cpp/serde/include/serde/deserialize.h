#pragma once

#include "result.h"
#include "error.h"

namespace serde {

template<typename T>
void deserialize(class Deserializer& de, T& v);

} // namespace serde

