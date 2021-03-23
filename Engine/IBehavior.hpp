#pragma once

#include <EngineExport.hpp>
#include <Interface.hpp>

namespace Engine {
    class ENGINE_EXPORT IBehavior : public Interface {
    public:
        virtual void Start() = 0;
        virtual void Update() = 0;
    };
}