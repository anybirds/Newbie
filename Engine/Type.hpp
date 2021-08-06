#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <nlohmann/json.hpp>

#include <EngineExport.hpp>

#define IS(Type, Member, Property) template <typename T = Type> T const &Is##Property() const { return Member; }
#define GET(Type, Member, Property) template <typename T = Type> T const &Get##Property() const { return Member; }
#define SET(Type, Member, Property) template <typename T = Type> void Set##Property(T const &Member) { this->Member = Member; }

#define PROPERTY_NONE_ACCESS_ATTR(Type, Member, Property, MemberAccess, Attr) MemberAccess: Attr Type Member
#define PROPERTY_NONE_ACCESS(Type, Member, Property, MemberAccess) PROPERTY_NONE_ACCESS_ATTR(Type, Member, Property, MemberAccess, )
#define PROPERTY_NONE_ATTR(Type, Member, Property, Attr) PROPERTY_NONE_ACCESS_ATTR(Type, Member, Property, private, Attr)
#define PROPERTY_NONE(Type, Member, Property) PROPERTY_NONE_ACCESS(Type, Member, Property, private)

#define PROPERTY_GET_ACCESS_ATTR(Type, Member, Property, MemberAccess, GetAccess, Attr) GetAccess: GET(Type, Member, Property) MemberAccess: Attr Type Member
#define PROPERTY_GET_ACCESS(Type, Member, Property, MemberAccess, GetAccess) PROPERTY_GET_ACCESS_ATTR(Type, Member, Property, MemberAccess, GetAccess, )
#define PROPERTY_GET_ATTR(Type, Member, Property, Attr) PROPERTY_GET_ACCESS_ATTR(Type, Member, Property, private, public, Attr)
#define PROPERTY_GET(Type, Member, Property) PROPERTY_GET_ACCESS(Type, Member, Property, private, public)
#define PROPERTY_IS_ACCESS_ATTR(Type, Member, Property, MemberAccess, IsAccess, Attr) IsAccess: IS(Type, Member, Property) MemberAccess: Attr Type Member
#define PROPERTY_IS_ACCESS(Type, Member, Property, MemberAccess, IsAccess) PROPERTY_IS_ACCESS_ATTR(Type, Member, Property, MemberAccess, IsAccess, )
#define PROPERTY_IS_ATTR(Type, Member, Property, Attr) PROPERTY_IS_ACCESS_ATTR(Type, Member, Property, private, public, Attr)
#define PROPERTY_IS(Type, Member, Property) PROPERTY_IS_ACCESS(Type, Member, Property, private, public)

#define PROPERTY_SET_ACCESS_ATTR(Type, Member, Property, MemberAccess, SetAccess, Attr) SetAccess: SET(Type, Member, Property) MemberAccess: Attr Type Member
#define PROPERTY_SET_ACCESS(Type, Member, Property, MemberAccess, SetAccess) PROPERTY_SET_ACCESS_ATTR(Type, Member, Property, MemberAccess, SetAccess, )
#define PROPERTY_SET_ATTR(Type, Member, Property, Attr) PROPERTY_SET_ACCESS_ATTR(Type, Member, Property, private, public, Attr)
#define PROPERTY_SET(Type, Member, Property) PROPERTY_SET_ACCESS(Type, Member, Property, private, public)

#define PROPERTY_ACCESS_ATTR(Type, Member, Property, MemberAccess, GetAccess, SetAccess, Attr) GetAccess: GET(Type, Member, Property) SetAccess: SET(Type, Member, Property) MemberAccess: Attr Type Member
#define PROPERTY_ACCESS(Type, Member, Property, MemberAccess, GetAccess, SetAccess) PROPERTY_ACCESS_ATTR(Type, Member, Property, MemberAccess, GetAccess, SetAccess, )
#define PROPERTY_ATTR(Type, Member, Property, Attr) PROPERTY_ACCESS_ATTR(Type, Member, Property, private, public, public, Attr)
#define PROPERTY(Type, Member, Property) PROPERTY_ACCESS(Type, Member, Property, private, public, public)
#define PROPERTY_BOOL_ACCESS_ATTR(Type, Member, Property, MemberAccess, IsAccess, SetAccess, Attr) IsAccess: IS(Type, Member, Property) SetAccess: SET(Type, Member, Property) MemberAccess: Attr Type Member
#define PROPERTY_BOOL_ACCESS(Type, Member, Property, MemberAccess, IsAccess, SetAccess) PROPERTY_BOOL_ACCESS_ATTR(Type, Member, Property, MemberAccess, IsAccess, SetAccess, )
#define PROPERTY_BOOL_ATTR(Type, Member, Property, Attr) PROPERTY_BOOL_ACCESS_ATTR(Type, Member, Property, private, public, public, Attr)
#define PROPERTY_BOOL(Type, Member, Property) PROPERTY_BOOL_ACCESS(Type, Member, Property, private, public, public)

#define CLASS_ENTITY(Entity, Empty, Attr) class Attr Entity 
#define CLASS_FINAL_ATTR(Derived, Base, Attr) class Attr Derived final : public Base
#define CLASS_FINAL(Derived, Base) CLASS_FINAL_ATTR(Derived, Base, )
#define CLASS_ATTR(Derived, Base, Attr) class Attr Derived : public Base
#define CLASS(Derived, Base) CLASS_ATTR(Derived, Base, )
#define CLASS_RESOURCE_ATTR(Derived, Base, Attr) CLASS_ATTR(Derived, Base, Attr)
#define CLASS_RESOURCE(Derived, Base) CLASS(Derived, Base)

#define ENUM(Derived, Base) enum class Derived : Base

#define NAMESPACE_ATTR(Name, Attr) namespace Attr Name 
#define NAMESPACE(Name) NAMESPACE_ATTR(Name, )

#define TYPE(T) \
    public: \
    static Type *StaticType() { return ::GetType<T>(); } \
    virtual Type *GetType() const { return T::StaticType(); } \
    static void Serialize(nlohmann::json &, const Entity *); \
    static size_t Deserialize(const nlohmann::json &, Entity *); \
    static void Visualize(Entity *) \

class Entity;

class ENGINE_EXPORT Type final {
public:
    static std::unordered_map<std::string, Type *> &GetAllTypes() {
        static std::unordered_map<std::string, Type *> types;
        return types;
    }
    static Type *GetType(const std::string &name) {
        auto &types = GetAllTypes(); 
        auto it = types.find(name);
        if (it == types.end()) {
            return nullptr;
        }
        return it->second;
    }
    static bool IsBaseOf(Type *base, Type *derived) {
        while (derived) {
            if (derived == base) { return true; }
            derived = derived->GetBase();
        }
        return false;
    }

private:
    bool enumeration;
    bool abstract;
    std::string name;
    Type *base;
    nlohmann::json blueprint;
    std::function<Entity *()> instantiate;
    std::function<void(nlohmann::json &, const Entity *)> serialize;
    std::function<size_t(const nlohmann::json &, Entity *)> deserialize;
    std::function<void(Entity *)> visualize;

    void SetEnum(bool enumeration) { this->enumeration = enumeration; }
    void SetAbstract(bool abstract) { this->abstract = abstract; }
    void SetBase(Type *base) { this->base = base; }
    void SetInstantiate(const std::function<Entity *()> &instantiate) { this->instantiate = instantiate; }
    void SetSerialize(const std::function<void(nlohmann::json &, const Entity *)> &serialize) { this->serialize = serialize; }
    void SetDeserialize(const std::function<size_t(const nlohmann::json &, Entity *)> &deserialize) { this->deserialize = deserialize; }
    void SetVisualize(const std::function<void(Entity *)> &visualize) { this->visualize = visualize; }

public:
    Type(const std::string &name);
    ~Type();

    bool IsEnum() const { return enumeration; }
    bool IsAbstract() const { return abstract; }
    const std::string &GetName() const { return name; }
    Type *GetBase() const { return base; }
    const nlohmann::json &GetBlueprint() const { return blueprint; }

    Entity *Instantiate() const { return instantiate(); }
    void Serialize(nlohmann::json &js, const Entity *entity) const { serialize(js, entity); }
    size_t Deserialize(const nlohmann::json &js, Entity *entity) const { return deserialize(js, entity); }
    void Visualize(Entity *entity) const { visualize(entity); }

    friend void ::type_init();
};

template <typename T>
Type *&GetType() { static Type *type; return type; }

/* serialize and deserialize glm types */
namespace glm {
    void to_json(nlohmann::json &js, const glm::vec2 &v);
    void to_json(nlohmann::json &js, const glm::vec3 &v);
    void to_json(nlohmann::json &js, const glm::vec4 &v);
    void to_json(nlohmann::json &js, const glm::mat2 &m);
    void to_json(nlohmann::json &js, const glm::mat3 &m);
    void to_json(nlohmann::json &js, const glm::mat4 &m);
    void to_json(nlohmann::json &js, const glm::quat &q);
    void from_json(const nlohmann::json &js, glm::vec2 &v);
    void from_json(const nlohmann::json &js, glm::vec3 &v);
    void from_json(const nlohmann::json &js, glm::vec4 &v);
    void from_json(const nlohmann::json &js, glm::mat2 &m);
    void from_json(const nlohmann::json &js, glm::mat3 &m);
    void from_json(const nlohmann::json &js, glm::mat4 &m);
    void from_json(const nlohmann::json &js, glm::quat &q);
}

/* serialize and deserialize enums */
template <typename T, std::enable_if_t<std::is_enum<T>::value, bool> = true> 
void to_json(nlohmann::json &js, const T &t) {
    js = static_cast<std::underlying_type_t<T>>(t);
}

template <typename T, std::enable_if_t<std::is_enum<T>::value, bool> = true> 
void from_json(const nlohmann::json &js, T &t) {
    t = static_cast<T>(js.get<std::underlying_type_t<T>>());
}