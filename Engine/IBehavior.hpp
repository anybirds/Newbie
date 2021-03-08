#pragma once

#include <EngineExport.hpp>

namespace Engine {
    class ENGINE_EXPORT IBehavior {
    public:
        virtual void Start() = 0;
        virtual void Update() = 0;
    };
}