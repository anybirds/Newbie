#pragma once

#include <Engine.hpp>

CLASS(SampleScript, Script) {
    TYPE(SampleScript);

    PROPERTY(float, rate, Rate);
    PROPERTY(std::vector<int>, vec, Vec);
    
public:
    virtual void Start() override;
    virtual void Update() override;
};