#pragma once

#include <EngineExport.hpp>

namespace Engine {
    class ENGINE_EXPORT IEnable {
    public:
        virtual void OnEnable() = 0;
        virtual void OnDisable() = 0;
    };
}