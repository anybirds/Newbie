#pragma once

#include <Engine.hpp>
#include <UserExport.hpp>

CLASS_ATTR(SampleScript, Script, USER_EXPORT) {
    TYPE(SampleScript);

    PROPERTY(float, rate, Rate);

public:
    virtual void Start() override;
    virtual void Update() override;
};