#pragma once

#include <Component.hpp>

namespace Engine {
    class ENGINE_EXPORT [[Serialize]] Script : public Component {
        TYPE(Script)

    public:
        virtual void Start() {}
        virtual void Update() {}
    };
}