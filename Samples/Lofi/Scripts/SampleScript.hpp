#pragma once

#include <Engine.hpp>

CLASS(SampleScript, Script) {
    TYPE(SampleScript);

    PROPERTY(float, rate, Rate);
    PROPERTY(std::set<int>, set, Set);
    PROPERTY(IntMap, intMap, IntMap);
    
public:
    virtual void Start() override;
    virtual void Update() override;
};