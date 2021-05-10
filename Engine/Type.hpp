#pragma once

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <nlohmann/json.hpp>

#include <EngineExport.hpp>

#define GET(Type, Member, Property) Type const &Get##Property() const { return Member; }
#define SET(Type, Member, Property) void Set##Property(Type const &Member) { this->Member = Member; }

#define PROPERTY_NONE_ACCESS_ATTR(Type, Member, MemberAccess, Attr) MemberAccess: Attr Type Member
#define PROPERTY_NONE_ACCESS(Type, Member, MemberAccess) PROPERTY_NONE_ACCESS_ATTR(Type, Member, MemberAccess, )
#define PROPERTY_NONE_ATTR(Type, Member, Attr) PROPERTY_NONE_ACCESS_ATTR(Type, Member, private, Attr)
#define PROPERTY_NONE(Type, Member) PROPERTY_NONE_ACCESS(Type, Member, private)
#define PROPERTY_GET_ACCESS_ATTR(Type, Member, Property, MemberAccess, GetAccess, Attr) GetAccess: GET(Type, Member, Property) MemberAccess: Attr Type Member
#define PROPERTY_GET_ACCESS(Type, Member, Property, MemberAccess, GetAccess) PROPERTY_GET_ACCESS_ATTR(Type, Member, Property, MemberAccess, GetAccess, )
#define PROPERTY_GET_ATTR(Type, Member, Property, Attr) PROPERTY_GET_ACCESS_ATTR(Type, Member, Property, private, public, Attr)
#define PROPERTY_GET(Type, Member, Property) PROPERTY_GET_ACCESS(Type, Member, Property, private, public)
#define PROPERTY_SET_ACCESS_ATTR(Type, Member, Property, MemberAccess, SetAccess, Attr) SetAccess: SET(Type, Member, Property) MemberAccess: Attr Type Member
#define PROPERTY_SET_ACCESS(Type, Member, Property, MemberAccess, SetAccess) PROPERTY_SET_ACCESS_ATTR(Type, Member, Property, MemberAccess, SetAccess, )
#define PROPERTY_SET_ATTR(Type, Member, Property, Attr) PROPERTY_SET_ACCESS_ATTR(Type, Member, Property, private, public, Attr)
#define PROPERTY_SET(Type, Member, Property) PROPERTY_SET_ACCESS(Type, Member, Property, private, public)
#define PROPERTY_GET_SET_ACCESS_ATTR(Type, Member, Property, MemberAccess, GetAccess, SetAccess, Attr) GetAccess: GET(Type, Member, Property) SetAccess: SET(Type, Member, Property) MemberAccess: Attr Type Member
#define PROPERTY_GET_SET_ACCESS(Type, Member, Property, MemberAccess, GetAccess, SetAccess) PROPERTY_GET_SET_ACCESS_ATTR(Type, Member, Property, MemberAccess, GetAccess, SetAccess, )
#define PROPERTY_GET_SET_ATTR(Type, Member, Property, Attr) PROPERTY_GET_SET_ACCESS_ATTR(Type, Member, Property, private, public, public, Attr)
#define PROPERTY_GET_SET(Type, Member, Property) PROPERTY_GET_SET_ACCESS(Type, Member, Property, private, public, public)
#define PROPERTY_ATTR(Type, Member, Property, Attr) PROPERTY_GET_SET_ATTR(Type, Member, Property, Attr)
#define PROPERTY(Type, Member, Property) PROPERTY_GET_SET(Type, Member, Property)

#define CLASS_ENTITY(Entity, Empty, Attr) class Attr Entity 
#define CLASS_FINAL_ATTR(Derived, Base, Attr) class Attr Derived final : public Base
#define CLASS_FINAL(Derived, Base) CLASS_FINAL_ATTR(Derived, Base, )
#define CLASS_ATTR(Derived, Base, Attr) class Attr Derived : public Base
#define CLASS(Derived, Base) CLASS_ATTR(Derived, Base, )

#define NAMESPACE_ATTR(Name, Attr) namespace Attr Name 
#define NAMESPACE(Name) NAMESPACE_ATTR(Name, )

#define TYPE(T) \
    public: \
    static Type *&StaticType() { static Type *type; return type; } \
    static void SetType(Type *type) { StaticType() = type; } \
    virtual Type *GetType() const { return T::StaticType(); } \
    private: \
    static void Serialize(nlohmann::json &, const Entity *); \
    static size_t Deserialize(const nlohmann::json &, Entity *); \
    friend void ::type_init() 
    

/* serializing and deserializing possible for glm vector and matrix types */
namespace glm {
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::vec3 &v);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::vec4 &v);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::mat3 &m);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::mat4 &m);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::quat &q);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::vec3 &v);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::vec4 &v);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::mat3 &m);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::mat4 &m);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::quat &q);
}

class Entity;

template <typename T,
std::enable_if_t<std::is_base_of_v<Entity, T> && !std::is_abstract_v<T>, bool> = true>
Entity *instantiate() {
    return (Entity *)new T();
}
template <typename T,
std::enable_if_t<std::is_base_of_v<Entity, T> && std::is_abstract_v<T>, bool> = true>
Entity *instantiate() {
    return nullptr;
}

class ENGINE_EXPORT Type final {
public:
    typedef Entity *(*FInstantiate)();
    typedef void (*FSerialize)(nlohmann::json &, const Entity *);
    typedef size_t (*FDeserialize)(const nlohmann::json &, Entity *);

private:
    static std::unordered_map<std::string, const Type *> &GetAllTypes() {
        static std::unordered_map<std::string, const Type *> types;
        return types;
    }

public:
    static const Type *GetType(const std::string &name) {
        auto &types = GetAllTypes();
        auto it = types.find(name);
        if (it == types.end()) {
            return nullptr;
        }
        return it->second;
    }

private:
    std::string name;
    FInstantiate instantiate;
    FSerialize serialize;
    FDeserialize deserialize;

public:
    Type(const std::string &name);
    ~Type();

    const std::string &GetName() const { return name; }

    void SetInstantiate(FInstantiate instantiate) { this->instantiate = instantiate; }
    void SetSerialize(FSerialize serialize) { this->serialize = serialize; }
    void SetDeserialize(FDeserialize deserialize) { this->deserialize = deserialize; }

    Entity *Instantiate() const { return instantiate(); }
    void Serialize(nlohmann::json &js, const Entity *entity) const { serialize(js, entity); }
    size_t Deserialize(const nlohmann::json &js, Entity *entity) const { return deserialize(js, entity); }
};