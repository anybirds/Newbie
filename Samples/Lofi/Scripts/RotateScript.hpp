#pragma once

#include <Engine.hpp>
#include <UserExport.hpp>

class USER_EXPORT [[Serialize]] RotateScript : Engine::Script {
    TYPE(RotateScript)

private:
    float rate;

public:
    virtual void Update() override;
};
 