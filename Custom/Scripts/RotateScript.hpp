#ifndef ROTATE_SCRIPT_H
#define ROTATE_SCRIPT_H

#include <Common/Component.hpp>
#include <Common/Script.hpp>

#include <custom_global.hpp>

using namespace Engine;

SER_DECL(CUSTOM_EXPORT, RotateScript)

class CUSTOM_EXPORT RotateScript : public Component, public Script {
    TYPE_DECL(RotateScript)

    PROPERTY(float, Rate, rate)
    float elapsed;

public:
    RotateScript(const std::string &name, Type *type = RotateScript::type);

    virtual void OnUpdate() override;
};

typedef typename concat<TYPE_LIST, RotateScript>::type TypeListRotateScript;
#undef TYPE_LIST
#define TYPE_LIST TypeListRotateScript

#endif
