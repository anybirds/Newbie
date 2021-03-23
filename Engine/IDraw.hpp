#pragma once

#include <EngineExport.hpp>
#include <Interface.hpp>

namespace Engine {
    
    class Camera;

    class ENGINE_EXPORT IDraw : public Interface {
    public:
        virtual void Draw(Camera *camera) = 0;
    };
}