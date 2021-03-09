#pragma once

#include <EngineExport.hpp>

namespace Engine {
    
    class Camera;

    class ENGINE_EXPORT IDraw {
    public:
        virtual void Draw(Camera *camera) = 0;
    };
}