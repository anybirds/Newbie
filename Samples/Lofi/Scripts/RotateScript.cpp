#include "RotateScript.hpp"

using namespace glm;
using namespace Engine;

void RotateScript::Update() {
    float dt = Time::GetInstance().GetDeltaTime();
    Transform *transform = GetTransform();
    transform->RotateAround(vec3(0.0f, 0.0f, 1.0f), rate * dt); 
}