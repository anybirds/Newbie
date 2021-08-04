#pragma once

#include <Script.hpp>

CLASS(EmptyScript, Script) {
    TYPE(EmptyScript);

public:
    virtual void Start() override;
    virtual void Update() override;
};