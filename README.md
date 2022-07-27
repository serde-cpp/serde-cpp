serde-cpp
===

> Currently in first stage of development !!
> Open to contributions!

Serialization framework for C++17, inspired on Rust's [serde](https://serde.rs/).

**serde-cpp** ultimate goal is to generate serialization code for C++ data structures without MACROS,
but rather with only valid C++17 syntax (using attributes), and along with that, serialize to
any data format using a single serialization API, like serde does for Rust.

Roadmap
---

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
  - [ ] unordered\_multiset, unordered\_multimap
  - [ ] list, forward\_list
  - [ ] stack, deque, queue, priority\_queue
  - [ ] initializer\_list
  - [ ] any
- [ ] Serde generation with attributes for user types
  - [ ] enum
  - [ ] struct (POD type)
  - [ ] forward-declarations
  - [ ] foreign-types
  - [ ] additional attributes (skip, rename, getter, setter, ...)
- [ ] Validate serde calls (seq/map utils) at interface level before impl (protected virtual)
- [ ] Serde Union support
- [ ] serde test suite
  - [ ] Invalid hierarchy (map/seq/struct..)
  - [ ] User Complex types
- [ ] serde\_attr test suite
  - [ ] Simple types
  - [ ] Template types
- [ ] serde\_yaml test suite
  - [ ] full api test
- [ ] CMake package
