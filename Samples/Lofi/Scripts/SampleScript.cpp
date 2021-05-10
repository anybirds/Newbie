#include "SampleScript.hpp"

using namespace std;
using namespace glm;

void SampleScript::Start() {

}

void SampleScript::Update() {
    float dt = Time::GetInstance().GetDeltaTime();
    Transform *transform = GetTransform();
    transform->RotateAround(vec3(0.0f, 0.0f, 1.0f), rate * dt);
}