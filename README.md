serde-cpp
===

> Currently in first stage of development !!
> Open to contributions!

Serialization framework for C++17, based on [serde](https://serde.rs/) for Rust.

**serde-cpp** ultimate goal is to generate serialization code for C++ data structures without MACROS,
but rather with only valid C++17 syntax (using attributes), and along with that, serialize to
any data format using a single serialization API, like serde does for Rust.

Roadmap
---

- [x] Serializer interface
- [ ] Deserializer interface
- [ ] Builtin de/serializers
  - [x] yaml
  - [ ] json
  - [ ] toml
  - [ ] xml
  - [ ] protobuf/flatbuffers/flexbuffers?
- [ ] Builtin std types serialization 
  - [ ] string, string\_view
  - [ ] vector, array
  - [ ] map, unordered\_map
  - [ ] set, unordered\_set
  - [ ] tuple, pair
  - [ ] optional
  - [ ] variant
- [ ] Serde generation with attributes for user types
  - [ ] enum
  - [ ] struct (POD type)
  - [ ] forward-declarations
  - [ ] foreign-types
  - [ ] additional attributes (skip, rename, getter, setter, ...)
- [ ] Validate serde calls (seq/map utils) at interface level before impl (protected virtual)
