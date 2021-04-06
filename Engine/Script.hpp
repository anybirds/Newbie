#pragma once

#include <Component.hpp>

NAMESPACE(Engine) {
    CLASS_ATTR(Script, Component, ENGINE_EXPORT) {
        TYPE(Script);

    public:
        virtual void Start() {}
        virtual void Update() {}
    };
}