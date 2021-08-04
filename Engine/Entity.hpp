#pragma once

#include <cstdint>
#include <unordered_map>
#include <type_traits>

#include <Type.hpp>

CLASS_ENTITY(Entity, , ENGINE_EXPORT) {
    TYPE(Entity);

public:
    static std::unordered_map<uintptr_t, uintptr_t> &GetMap() {
        static std::unordered_map<uintptr_t, uintptr_t> map;
        return map;
    }
    static bool &GetNullify() { static bool nullify; return nullify; }
    static void SetNullify(bool nullify) { GetNullify() = nullify; }

public:
    virtual ~Entity() = 0 {}
};

class Asset;

void ENGINE_EXPORT from_json(const nlohmann::json &js, Entity *&entity);

template <typename T, std::enable_if_t<std::is_base_of_v<Entity, T> &&
!std::is_base_of_v<Asset, T>, bool> = true>
void to_json(nlohmann::json &js, const T *t) {
    js = (uint64_t)t;
}

template <typename T, std::enable_if_t<std::is_base_of_v<Entity, T> &&
!std::is_base_of_v<Asset, T>, bool> = true> 
void from_json(const nlohmann::json &js, T *&t) {
    Entity *entity = nullptr;
    from_json(js, entity);
    t = (T *)entity;
}