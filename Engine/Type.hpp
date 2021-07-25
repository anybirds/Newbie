#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <nlohmann/json.hpp>

#include <EngineExport.hpp>

#define IS(Type, Member, Property) template <typename T = Type> T const &Is##Property() const { return Member; }
#define GET(Type, Member, Property) template <typename T = Type> T const &Get##Property() const { return Member; }
#define SET(Type, Member, Property) template <typename T = Type> void Set##Property(T const &Member) { this->Member = Member; }

#define PROPERTY_NONE_ACCESS_ATTR(Type, Member, MemberAccess, Attr) MemberAccess: Attr Type Member
#define PROPERTY_NONE_ACCESS(Type, Member, MemberAccess) PROPERTY_NONE_ACCESS_ATTR(Type, Member, MemberAccess, )
#define PROPERTY_NONE_ATTR(Type, Member, Attr) PROPERTY_NONE_ACCESS_ATTR(Type, Member, private, Attr)
#define PROPERTY_NONE(Type, Member) PROPERTY_NONE_ACCESS(Type, Member, private)

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
    static void Visualize(Entity *); \
    friend void ::type_init() 
    
class Entity;

class ENGINE_EXPORT Type final {
private:
    static std::unordered_map<std::string, Type *> &GetAllTypes() {
        static std::unordered_map<std::string, Type *> types;
        return types;
    }

public:
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
    std::string name;
    Type *base;
    std::function<Entity *()> instantiate;
    std::function<void(nlohmann::json &, const Entity *)> serialize;
    std::function<size_t(const nlohmann::json &, Entity *)> deserialize;
    std::function<void(Entity *)> visualize;

public:
    Type(const std::string &name);
    ~Type();

    const std::string &GetName() const { return name; }
    Type *GetBase() const { return base; }
    void SetBase(Type *base) { this->base = base; }

    void SetInstantiate(const std::function<Entity *()> &instantiate) { this->instantiate = instantiate; }
    void SetSerialize(const std::function<void(nlohmann::json &, const Entity *)> &serialize) { this->serialize = serialize; }
    void SetDeserialize(const std::function<size_t(const nlohmann::json &, Entity *)> &deserialize) { this->deserialize = deserialize; }
    void SetVisualize(const std::function<void(Entity *)> &visualize) { this->visualize = visualize; }

    Entity *Instantiate() const { return instantiate(); }
    void Serialize(nlohmann::json &js, const Entity *entity) const { serialize(js, entity); }
    size_t Deserialize(const nlohmann::json &js, Entity *entity) const { return deserialize(js, entity); }
    void Visualize(Entity *entity) const { visualize(entity); }
};