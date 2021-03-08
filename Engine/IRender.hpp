#pragma once

#include <EngineExport.hpp>

namespace Engine {
    class ENGINE_EXPORT IRender {
    public:
        virtual void Render() = 0;
    };
}