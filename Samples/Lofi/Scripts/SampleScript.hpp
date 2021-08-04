#pragma once

#include <vector>

#include <Script.hpp>

CLASS(SampleScript, Script) {
    TYPE(SampleScript);

    PROPERTY(float, rate, Rate);
    
public:
    virtual void Start() override;
    virtual void Update() override;
};