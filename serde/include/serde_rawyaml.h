#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <stack>
#include <list>
#include "serde.h"
#include <assert.h>
#include <cstring>
#include <variant>
#include <vector>
#include <map>
#include <memory>
#include <optional>

namespace serde_rawyaml {

class YamlSerializer final : public serde::Serializer {

  struct Token {
    enum class Kind {
      Seq,
      Map,
      Entry,
    };
    Kind kind;
    serde::Style style;
    size_t indent = 0;
    size_t count = 0;
  };

  void handle_prefix(bool complex = false, serde::Style style = serde::Style::Inline) {
    if (!stack.empty()) {
      Token& token = stack.top();

      switch (token.kind) {
        case Token::Kind::Seq: {
          if (token.style == serde::Style::Inline) {
            assert(!(complex && style == serde::Style::Fold));
            if (++token.count > 1)
              ss << ", ";
          } else if (token.style == serde::Style::Fold) {
            if (++token.count > 1)
              ss << '\n' << std::setw(token.indent + 2);
            ss << "- ";
          }
          break;
        }

        case Token::Kind::Map: {
          if (token.style == serde::Style::Inline) {
            assert(!(complex && style == serde::Style::Fold));
            if (++token.count > 1)
              ss << ", ";
          } else if (token.style == serde::Style::Fold) {
            if (++token.count > 1)
              ss << '\n' << std::setw(token.indent) << "";
          }
          break;
        }

        case Token::Kind::Entry: {
          if (token.style == serde::Style::Fold) {
            if (++token.count > 1 && complex)
              ss << '\n' << std::setw(token.indent) << "";
          }
        }
      }
    }
  }

  void stack_push(Token::Kind kind, serde::Style style) {
    size_t indent = 0;
    if (!stack.empty()) {
      auto& top = stack.top();
      indent = top.indent;
      if (top.kind != Token::Kind::Entry)
        indent += 2;
    }
    stack.push({kind, style, indent, 0});
  }

  void serialize_bool(bool v) final {
    handle_prefix();
    ss << std::boolalpha << v << std::noboolalpha;
  }

  void serialize_int(int v) final {
    handle_prefix();
    ss << v;
  }

  void serialize_cstr(const char* v) final {
    handle_prefix();
    ss << v;
  }

  void serialize_none() final {
    handle_prefix();
    ss << "null";
  }

  void serialize_seq_begin(serde::Style style = serde::Style::Fold) final {
    handle_prefix(true, style);
    if (style == serde::Style::Inline)
      ss << '[';
    stack_push(Token::Kind::Seq, style);
  }

  void serialize_seq_end() final {
    if (!stack.empty()) {
      auto& top = stack.top();
      if (top.kind != Token::Kind::Seq)
        return;
      if (top.style == serde::Style::Inline)
        ss << ']';
      else if (top.style == serde::Style::Fold && stack.size() == 1)
        ss << '\n';
      stack.pop();
    }
  }

  void serialize_map_begin(serde::Style style = serde::Style::Fold) final {
    handle_prefix(true, style);
    if (style == serde::Style::Inline)
      ss << '{';
    stack_push(Token::Kind::Map, style);
  }

  void serialize_map_end() final {
    if (!stack.empty()) {
      auto& top = stack.top();
      if (top.kind != Token::Kind::Map)
        return;
      if (top.style == serde::Style::Inline)
        ss << '}';
      else if (top.style == serde::Style::Fold && stack.size() == 1)
        ss << '\n';
      stack.pop();
    }
  }

  void serialize_map_key_begin() final {
    if (!stack.empty()) {
      auto& top = stack.top();
      if (top.kind != Token::Kind::Map)
        return;
      handle_prefix();
      stack_push(Token::Kind::Entry, top.style);
    }
  }
  void serialize_map_key_end() final {
    ss << ": ";
  }

  void serialize_map_value_begin() final {
  }
  void serialize_map_value_end() final {
    stack.pop();
  }

  void serialize_struct_begin(serde::Style style = serde::Style::Fold) final {
    serialize_map_begin(style);
  }
  void serialize_struct_end() final {
    serialize_map_end();
  }

  void serialize_struct_field_begin(const char* name) final {
    serialize_map_key_begin();
    serialize(name);
    serialize_map_key_end();
  }
  void serialize_struct_field_end() final {
    stack.pop();
  }

public:

  YamlSerializer() {
    ss << "---\n";
  }
  ~YamlSerializer() = default;

  std::string str() { return ss.str(); }

private:
  std::stringstream ss;
  std::stack<Token> stack;
};

template<typename T>
auto to_string(T&& obj) -> Result<std::string, serde::Error>
{
  auto ser = YamlSerializer();
  ser.serialize(obj);
  return Ok(ser.str());
}

// ==================================================================

class YamlDeserializer final : public serde::Deserializer {

  enum class Token {
    SeqBegin,
    SeqEnd,
    MapBegin,
    MapEnd,
    Scalar,
  };

  struct Node;

  using Seq = std::vector<std::shared_ptr<Node>>;
  using Map = std::map<std::string, std::shared_ptr<Node>>;
  using Scalar = std::string;
  using Variant = std::variant<Seq, Map, Scalar>;

  struct Node {
    Variant value{};
  };

public:
  YamlDeserializer(std::string&& yaml)
    : yaml(std::move(yaml)), ss(this->yaml)
  {
    ss.exceptions(std::ios_base::goodbit);
  }
  ~YamlDeserializer() = default;

  Result parse() {
    for (char ch = ss.peek(); ss.good(); ch = ss.peek()) {
      std::cout << "  " << int(ch) << "(" << ch << ")"
                << " pos: " << ss.tellg() << std::endl;
      bool got = false;
      switch (ch) {
        case 'a'...'z': [[fallthrough]];
        case 'A'...'B': [[fallthrough]];
        case '0'...'9': got = parse_scalar(); break;
        case '-':  got = parse_hyphen(); break;
        case ':': break; // key
        case ',': break; // entry end
        case '\n': break; // key/value end
        case ' ': break; // space or str
        default: break;
      }
      if (!got) ss.get();
    }
    return Ok();
  }

  bool parse_scalar() {
    int count = 0;
    for(char ch = ss.peek(); ss.good() || ss.eof(); ss.get(), ch = ss.peek()) {
      switch (ch) {
        case 'a'...'z': [[fallthrough]];
        case 'A'...'B': [[fallthrough]];
        case '0'...'9': count++; break;
        case ':': break; // key
        case ',': break; // entry end
        default: goto exit;
      }
    }
exit:
    ss.get();
    std::cout << ">> got scalar" << std::endl;
    return true;
  }

  bool parse_hyphen() {
    if (parse_doc_header()) return true;
    if (parse_seq()) return true;
    return false;
  }

  bool parse_doc_header() {
    auto g = ss.tellg();
    char str[5] = {0};
    ss.get(str, sizeof(str), '\0');
    if (ss.good() || ss.eof()) {
      if (!std::memcmp(str, "---\n", 4)) {
        std::cout << ">> got doc" << std::endl;
        return true;
      }
    } else std::cout << "~~ no good" << std::endl;
    std::cout << "!! no doc" << std::endl;
    ss.seekg(g);
    return false;
  }

  bool parse_seq() {
    auto g = ss.tellg();
    char str[3] = {0};
    ss.get(str, sizeof(str), '\0');
    if (ss.good() || ss.eof()) {
      if (str[0] == '-' && (str[1] == ' ' || str[1] == '\n')) {
        std::cout << ">> got seq" << std::endl;
        return true;
      }
    } else std::cout << "~~ no good" << std::endl;
    std::cout << "!! no seq" << std::endl;
    ss.seekg(g);
    return false;
  }

  void add_node(Token token, std::optional<Scalar> scalar) {
    auto add = [&] (std::shared_ptr<Node> node) {
      if (stack.empty()) return;
      auto curr = stack.top();
      if (auto seq = std::get_if<Seq>(&curr->value); seq) {
        seq->push_back(node);
      }
      if (auto map = std::get_if<Map>(&curr->value); map) {
        (*map)["key"] = node;
      }
    };

    std::shared_ptr<Node> node;
    switch (token) {
      case Token::SeqBegin: {
        node.reset(new Node{Seq{}});
        add(node);
        stack.push(node);
        break;
      }
      case Token::SeqEnd: {
        stack.pop();
        break;
      }
      case Token::MapBegin: break;
      case Token::MapEnd: break;
      case Token::Scalar: {
        node.reset(new Node{*scalar});
        add(node);
      }
    }
    if (stack.size() == 1) {
      root = stack.top();
    }
  }

private:
  std::string yaml;
  std::istringstream ss;
  std::shared_ptr<Node> root;
  std::stack<std::shared_ptr<Node>> stack;
};

template<typename T>
auto from_str(std::string str) -> Result<T, serde::Error>
{
  auto de = YamlDeserializer(std::move(str));
  de.parse();
  //de.deserialize();
  return Ok(T());
}

} // namespace serde_rawyaml
