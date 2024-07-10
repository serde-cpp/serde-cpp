serde-cpp
===

Automatic serialization code generator for C++17.

> Project under development!
> Looking for contributors.

## Example

Only need to add the `[[serde]]` attribute annotation to your **classes/structs/enums**.

```cpp
// main.cpp
#include <serde/serde_yaml.h>
#include "main_serde.h" // Generated file with serialization code

// Note the serde attribute here
struct [[serde]] Point {
  int x;
  int y;
};

int main() {
  Point p1{ 10, 20 };

  // Serialize to YAML
  std::string output = serde_yaml::to_string(p1).value();
  std::cout << output << std::endl;

  // Deserialize from YAML
  Point p2 = serde_yaml::from_str<Point>("x: 10\ny: 20\n").value();
  assert(p1.x == p2.x && p1.y == p2.y);
}
```

No need for macros, no duplicate declaration of data members, no extra code!
Isn't that beautiful?


## Summary

<details open>
<summary><b>README</b></summary>

* [Serde-cpp](#serde-cpp)
* [Example](#example)
* [Summary](#summary)
* [Features](#features)
* [How we do it?](#how-we-do-it)
* [Overview](#overview)
* [Project Structure](#project-structure)
* [Roadmap](#roadmap)
* [License](#license)

</details>


## Features

* Automatic serialization of any data structure (std's and user's types);
* C++17 syntax with `[[attributes]]`; See [cpp reference](https://en.cppreference.com/w/cpp/language/attributes)
* Single serialization and deserialization APIs;
* Multiple output formats (yaml, json, toml, ...);
* No MACROS required.


## How we do it?

**serde-cpp** generates serialization code for each header or translation unit passed to the serde generator binary.
_serde_gen_ will scan the input file for types that have `[[serde]]` attributes added to them and generate the serialization and deserialization code
using Serializer and Deserializer APIs.

For the [example](#example) above, the generated serialization/deserialization code would look like this:

```cpp
// main_serde.h
#include <serde/serde.h>

namespace serde {

template<>
void serialize(serde::Serializer& ser, const Point& point)
{
  ser.serialize_struct_begin();
    ser.serialize_struct_field("x", point.x);
    ser.serialize_struct_field("y", point.y);
  ser.serialize_struct_end();
}

template<>
void deserialize(serde::Deserializer& de, Point& point)
{
  de.deserialize_struct_begin();
    de.deserialize_struct_field("x", point.x);
    de.deserialize_struct_field("y", point.y);
  de.deserialize_struct_end();
}

}  // namespace serde
```

Then the backend implementation of the Serializer/Deserializer will resolve the data layout.

For example, the YAML Serializer is already implemented and built into the project.
So the output of this example using the YAML Serializer is the following:

```yaml
# output.yml
x: 10
y: 20
```

In order to generate the serde file having serialization/deserialization code for your types,
a CMake command is provided. Just add the files you want to generate code for and it will output
the serialization/deserialization code for them.

```cmake
# CMakeLists.txt
include(serde-cpp) # include serde cmake macros
serde_generate(example_serde
    # generate serialization/deserialization for the following listed files
    main.cpp # a header "main_serde.h" will be generated
)
add_executable(example)
target_sources(example PRIVATE
    main.cpp
)
target_link_libraries(example PRIVATE
  example_serde # to add include path of the generated file
  serde_yaml
  serde
)
```

See the [example project](https://github.com/serde-cpp/serde-cpp-example-package).

## Overview

![serde-diagram](extras/serde-diagram.png)


---

## Project Structure

<details open>
<summary><b>serde-cpp/serde-cpp</b></summary>

* [external](./external) - External library dependencies
* [extras](./extras) - Extra infrastructure files
* [serde-cpp](./serde-cpp) - Project source code subdirectory
    + [serde](./serde-cpp/serde) - Serde APIs only
    + [serde\_gen](./serde-cpp/serde_gen) - Serde auto-generation binary project
    + [serde\_yaml](./serde-cpp/serde_yaml) - YAML implementation of Serde APIs

</details>

---

## Roadmap

- [x] Serializer interface
- [x] Deserializer interface
- [x] Builtin de/serializers
  - [x] yaml
  - [ ] json
  - [ ] toml
  - [ ] xml
- [x] Deserialize complex types (template types)
- [x] Serde for local scope and private user types
- [x] Builtin std types serialization 
  - [x] string, string\_view
  - [x] vector, array
  - [x] map, unordered\_map
  - [x] set, unordered\_set
  - [x] tuple, pair
  - [x] optional
  - [x] variant
  - [x] multiset, multimap
  - [x] unordered\_multiset, unordered\_multimap
  - [x] list, forward\_list
  - [x] stack, deque, queue, priority\_queue
  - [x] initializer\_list
- [x] Test std types serialization
- [x] Test builtin types serialization
- [x] De/Serialization for incomplete simple/template types (struct/class/enum)
- [ ] De/Serialization for specialized types
- [x] CMake exported function to generate serde files automatically
- [ ] Proper parsing/emitting error return (use exceptions?)
- [ ] Serde generation with attributes for user types
  - [ ] enum
  - [ ] struct (POD type)
  - [ ] forward-declarations
  - [ ] foreign-types
  - [ ] additional attributes (skip, skip\_de, skip\_ser, rename, getter, setter, flatten, skip\_none, default, untagged, ...)
- [ ] Validate serde calls (seq/map utils) at interface level before impl (protected virtual)
- [ ] Serde Union support
- [ ] Support non-default constructable types
- [ ] serde test suite
  - [ ] Invalid hierarchy (map/seq/struct..)
  - [ ] User Complex types
- [ ] serde\_attr test suite
  - [ ] Simple types
  - [ ] Template types
- [ ] serde\_yaml test suite
  - [ ] full api test
- [ ] CMake package
- [ ] Support wchar\_t and other CharT
- [ ] Serde-cpp project sample repo (using CMake's find\_package and add\_subdirectory)
- [ ] Polymorphic objects and abstract classes (`[[serde::polymorphic]]`)


#### License

<sup>
Licensed under either of <a href="LICENSE-APACHE">Apache License, Version
2.0</a> or <a href="LICENSE-MIT">MIT license</a> at your option.
</sup>

<br>

<sub>
Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in serde-cpp by you, as defined in the Apache-2.0 license, shall be
dual licensed as above, without any additional terms or conditions.
</sub>
