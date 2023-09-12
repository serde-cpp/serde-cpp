#pragma once

#include <fstream>
#include <iomanip>
#include <memory>
#include <ostream>
#include <vector>
#include <algorithm>

namespace serde_gen {
namespace gen {

struct IoCtl {
    int indent_lvl = 0;

    IoCtl& indent_inc()
    {
        indent_lvl += 2;
        return *this;
    }

    IoCtl& indent_dec()
    {
        indent_lvl -= 2;
        if (indent_lvl < 0)
            indent_lvl = 0;
        return *this;
    }
};

struct Gen {
    virtual std::ostream& write(std::ostream& os, IoCtl& ctl) const = 0;
    virtual ~Gen() = default;
    virtual std::unique_ptr<Gen> to_gen() = 0;
    operator std::unique_ptr<Gen>() { return to_gen(); }
};

template<typename T>
struct GenT : public Gen {
    virtual std::unique_ptr<Gen> to_gen() override
    {
        return std::make_unique<T>(std::move(*reinterpret_cast<T*>(this)));
    }
};

struct FileHeader : public GenT<FileHeader> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << R"(/*
 * Serde-cpp generated header file.
 * DO NOT EDIT!!
 */
)";
        return os;
    }
};

struct Namespace : public GenT<Namespace> {
    std::string name;
    explicit Namespace(std::string&& name) : name(std::move(name)) {}
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "namespace " << name << ' ';
        return os;
    }
};

struct LineBreak : public GenT<LineBreak> {
    int count;
    LineBreak(int count = 1) : count(count) {}
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        for (int i = 0; i < count; i++)
            os << '\n';
        return os;
    }
};

struct BlockBegin : public GenT<BlockBegin> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "{";
        ctl.indent_inc();
        return os;
    }
};

struct BlockBeginNL : public GenT<BlockBeginNL> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "{\n";
        ctl.indent_inc();
        return os;
    }
};

struct BlockEnd : public GenT<BlockEnd> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "}";
        ctl.indent_dec();
        return os;
    }
};

struct BlockEndNL : public GenT<BlockEndNL> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "}\n";
        ctl.indent_dec();
        return os;
    }
};

struct BlockEndSemiColon : public GenT<BlockEndSemiColon> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "};";
        ctl.indent_dec();
        return os;
    }
};

struct BlockEndSemiColonNL : public GenT<BlockEndSemiColonNL> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "};\n";
        ctl.indent_dec();
        return os;
    }
};

struct CommentInline : public GenT<CommentInline> {
    std::string comment;
    explicit CommentInline(std::string&& comment) : comment(std::move(comment)) {}
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << " // " << comment;
        return os;
    }
};

struct StructSerializeBegin : public GenT<StructSerializeBegin> {
    std::string name;
    explicit StructSerializeBegin(std::string&& name) : name(std::move(name)) {}
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "template<typename T>\n";
        os << "struct Serialize<T, std::enable_if_t<std::is_same_v<T, " << name << ">>> {\n";
        return os;
    }
};

struct StructSerializeEnd : public GenT<StructSerializeEnd> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "};\n";
        ctl.indent_dec();
        return os;
    }
};

struct StructDeserializeBegin : public GenT<StructDeserializeBegin> {
    std::string name;
    explicit StructDeserializeBegin(std::string&& name) : name(std::move(name)) {}
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "template<typename T>\n";
        os << "struct Deserialize<T, std::enable_if_t<std::is_same_v<T, " << name << ">>> {\n";
        return os;
    }
};

struct StructDeserializeEnd : public GenT<StructDeserializeEnd> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "};\n";
        ctl.indent_dec();
        return os;
    }
};

struct ApiSerializeStructField : public GenT<ApiSerializeStructField> {
    std::string key, value;
    explicit ApiSerializeStructField(const std::string& key, const std::string value)
        : key(key), value(value)
    {
    }
    explicit ApiSerializeStructField(std::string&& key, std::string&& value)
        : key(std::move(key)), value(std::move(value))
    {
    }
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "ser.serialize_struct_field(\"" << key << "\", val." << value << ");\n";
        return os;
    }
};

struct ApiDeserializeStructField : public GenT<ApiDeserializeStructField> {
    std::string key, value;
    explicit ApiDeserializeStructField(const std::string& key, const std::string value)
        : key(key), value(value)
    {
    }
    explicit ApiDeserializeStructField(std::string&& key, std::string&& value)
        : key(std::move(key)), value(std::move(value))
    {
    }
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "de.deserialize_struct_field(\"" << key << "\", val." << value << ");\n";
        return os;
    }
};

struct GenString : public GenT<GenString> {
    std::string string;
    explicit GenString(std::string&& string) : string(std::move(string)) {}
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << string;
        return os;
    }
};

struct GenGroup : public GenT<GenGroup> {
    std::vector<std::unique_ptr<Gen>> vec;

    template<typename... Gens>
    explicit GenGroup(Gens&&... gens) : vec(sizeof...(Gens))
    {
        (vec.emplace_back(gens.to_gen()), ...);
    }

    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        for (const auto& gen : vec)
            gen->write(os, ctl);
        return os;
    }
};

struct GenVector : public std::vector<std::unique_ptr<Gen>>, public GenT<GenVector> {
    using Base = std::vector<std::unique_ptr<Gen>>;
    using Base::Base;

    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        for (const auto& gen : *this)
            gen->write(os, ctl);
        return os;
    }
};

class Generator {
   public:
    Generator() = default;

    Generator& add_header(std::unique_ptr<Gen>&& gen)
    {
        headers.emplace_back(std::move(gen));
        return *this;
    }

    Generator& add_include_system(std::string&& header)
    {
        auto it = std::find(includes_system.begin(), includes_system.end(), header);
        if (it == includes_system.end())
            includes_system.emplace_back(std::move(header));
        return *this;
    }

    Generator& add_include_local(std::string&& header)
    {
        auto it = std::find(includes_local.begin(), includes_local.end(), header);
        if (it == includes_local.end())
            includes_local.emplace_back(std::move(header));
        return *this;
    }

    Generator& add(std::unique_ptr<Gen>&& gen)
    {
        gens.emplace_back(std::move(gen));
        return *this;
    }

    void write(std::ostream& os)
    {
        IoCtl ctl;
        for (const auto& header : headers) {
            header->write(os, ctl);
        }
        LineBreak().write(os, ctl);
        for (const auto& include : includes_system) {
            os << "#include <" << include << ">\n";
        }
        LineBreak().write(os, ctl);
        for (const auto& include : includes_local) {
            os << "#include \"" << include << "\"\n";
        }
        LineBreak().write(os, ctl);
        for (const auto& gen : gens) {
            gen->write(os, ctl);
        }
        LineBreak().write(os, ctl);
    }

   private:
    std::vector<std::unique_ptr<Gen>> headers;
    std::vector<std::string> includes_system;
    std::vector<std::string> includes_local;
    std::vector<std::unique_ptr<Gen>> gens;
};

#define SIMPLE_GEN_TYPE(Type, data)                                      \
    struct Type : public GenT<Type> {                                    \
        std::ostream& write(std::ostream& os, IoCtl& ctl) const override \
        {                                                                \
            os << data;                                                  \
            return os;                                                   \
        }                                                                \
    };

SIMPLE_GEN_TYPE(StaticMethodDeserializeBegin,
                "static void deserialize(Deserializer& de, const T& val) {\n");
SIMPLE_GEN_TYPE(StaticMethodSerializeBegin,
                "static void serialize(Serializer& ser, const T& val) {\n");
SIMPLE_GEN_TYPE(ApiSerializeStructBegin, "ser.serialize_struct_begin();\n");
SIMPLE_GEN_TYPE(ApiSerializeStructEnd, "ser.serialize_struct_end();\n");
SIMPLE_GEN_TYPE(ApiDeserializeStructBegin, "de.deserialize_struct_end();\n");
SIMPLE_GEN_TYPE(ApiDeserializeStructEnd, "de.deserialize_struct_end();\n");

struct StaticMethodSerializeEnd : public GenT<StaticMethodSerializeEnd> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "}\n";
        ctl.indent_dec();
        return os;
    }
};

struct StaticMethodDeserializeEnd : public GenT<StaticMethodDeserializeEnd> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "}\n";
        ctl.indent_dec();
        return os;
    }
};

}  // namespace gen
}  // namespace serde_gen
