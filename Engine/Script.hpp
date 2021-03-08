#pragma once

#include <unordered_set>

#include <Component.hpp>
#include <IBehavior.hpp>
#include <IEnable.hpp>

namespace Engine {
    /*
    Abstract base class of all user-defined scripts
    */
    class ENGINE_EXPORT Script : public Component, public IBehavior, public IEnable {
    public:
        virtual void Start() override {}
        virtual void Update() override {}
        virtual void OnEnable() override {}
        virtual void OnDisable() override {}
    };
}
