#pragma once

#include <EngineExport.hpp>
#include <Interface.hpp>

namespace Engine {
    class ENGINE_EXPORT IRender : public Interface {
    public:
        virtual void Render() = 0;
    };
}