#include "RotateScript.hpp"

using namespace glm;
using namespace Engine;

void RotateScript::Start() {

}

void RotateScript::Update() {
    float dt = Time::DeltaTime();
    Transform *transform = GetGameObject()->GetTransform();
    transform->RotateAround(vec3(0.0f, 0.0f, 1.0f), rate * dt); 
}