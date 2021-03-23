#pragma once

#include <Engine.hpp>
#include <UserExport.hpp>

class USER_EXPORT [[Serialize]] RotateScript : [[Serialize]] public Engine::Component, public Engine::IBehavior {
    TYPE(RotateScript)

private:
    float rate;

public:
    virtual void Start() override;
    virtual void Update() override;
};
 