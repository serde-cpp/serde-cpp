#pragma once

#include <fstream>
#include <iomanip>
#include <memory>
#include <vector>

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

struct IncludeSystem : public GenT<IncludeSystem> {
    std::string path;
    explicit IncludeSystem(std::string&& path) : path(std::move(path)) {}
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "#include <" << path << ">\n";
        return os;
    }
};

struct IncludeLocal : public GenT<IncludeLocal> {
    std::string path;
    explicit IncludeLocal(std::string&& path) : path(std::move(path)) {}
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "#include \"" << path << "\"\n";
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
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << '\n';
        return os;
    }
};

struct BlockBegin : public GenT<BlockBegin> {
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
        os << "}\n";
        ctl.indent_dec();
        return os;
    }
};

struct BlockEndSemiColon : public GenT<BlockEnd> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "};\n";
        ctl.indent_dec();
        return os;
    }
};

struct StructSerialize : public GenT<StructSerialize> {
    std::string name;
    explicit StructSerialize(std::string&& name) : name(std::move(name)) {}
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "template<typename T>\n";
        os << "struct Serialize<T, std::enable_if_t<std::is_same_v<T, " << name << ">>> ";
        return os;
    }
};

struct SerializeFunction : public GenT<SerializeFunction> {
    std::ostream& write(std::ostream& os, IoCtl& ctl) const override
    {
        os << "static void serialize(Serializer& ser, const T& val)";
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
    std::ostream& os;
    IoCtl ctl;

   public:
    Generator(std::ostream& os) : os(os) {}

    Generator& operator<<(const Gen& gen)
    {
        if (ctl.indent_lvl)
          os << std::setw(ctl.indent_lvl);
        gen.write(os, ctl);
        return *this;
    }
};

inline const auto SERIALIZE_STRUCT_BEGIN = GenString("ser.serialize_struct_begin();\n");
inline const auto SERIALIZE_STRUCT_END = GenString("ser.serialize_struct_end();\n");

}  // namespace gen
}  // namespace serde_gen
