#pragma once

#include <Component.hpp>

CLASS_ATTR(Script, Component, ENGINE_EXPORT) {
    TYPE(Script);

private:
    bool start;
    
public:
    Script() : Component(), start(true) {}

    virtual void Start() {}
    virtual void Update() {}

    virtual void OnTrack() override final;
    virtual void OnUntrack() override final;

    friend class Scene;
};