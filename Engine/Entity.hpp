#pragma once

#include <unordered_map>

#include <Type.hpp>

NAMESPACE(Engine) {
    
    CLASS_ENTITY(Entity, , ENGINE_EXPORT) {
        TYPE(Entity);

    public:
        static std::unordered_map<uint64_t, uint64_t> &GetMap() {
            static std::unordered_map<uint64_t, uint64_t> map;
            return map;
        }
        static bool &GetNullify() { static bool nullify; return nullify; }
        static void SetNullify(bool nullify) { GetNullify() = nullify; }

    public:
        virtual ~Entity() {}
    };
}