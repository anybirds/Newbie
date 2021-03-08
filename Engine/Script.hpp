#pragma once

#include <unordered_set>

#include <Component.hpp>

namespace Engine {
    /*
    Abstract base class of all user-defined scripts
    */
    class ENGINE_EXPORT Script : public Component {
    public:
        virtual void Start() {}
        virtual void Update() {}
    };
}
