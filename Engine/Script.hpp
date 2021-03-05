#pragma once

#include <unordered_set>

#include <EngineExport.hpp>
#include <Entity.hpp>

namespace Engine {
    /*
    Base class of all user-defined scripts
    */
    class ENGINE_EXPORT Script : public Entity {
    public:
        virtual void Start() {}
        virtual void Update() {}
    };
}
