#pragma once

#include <Engine.hpp>
#include <UserExport.hpp>

CLASS_ATTR(RotateScript, Engine::Script, USER_EXPORT) {
    TYPE(RotateScript);

    PROPERTY(float, rate, Rate);

public:
    virtual void Update() override;
};