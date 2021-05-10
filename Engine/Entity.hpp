#pragma once

#include <unordered_map>

#include <Type.hpp>

CLASS_ENTITY(Entity, , ENGINE_EXPORT) {
    TYPE(Entity);

public:
    static std::unordered_map<uintptr_t, uintptr_t> &GetMap() {
        static std::unordered_map<uintptr_t, uintptr_t> map;
        return map;
    }

public:
    virtual ~Entity() {}
};