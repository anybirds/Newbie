#include <Common/GameObject.hpp>
#include <Common/Time.hpp>
#include <Common/Transform.hpp>
#include <Common/Scene.hpp>

#include <Scripts/RotateScript.hpp>

using namespace glm;
using namespace std;

TYPE_DEF(RotateScript)
SER_DEF(RotateScript, Component,
MEMBER_SER | MEMBER_SHOW, float, rate
)

RotateScript::RotateScript(const string &name, Type *type) : Component(name, type), elapsed(0.0f) {

}

void RotateScript::OnUpdate() {
    float dt = Time::DeltaTime();
    elapsed += dt;
    if (elapsed > 5.0f) {
        Destroy(GetGameObject());
        Scene::Save();
    }
    Transform *transform = GetGameObject()->GetTransform();
    transform->RotateAround(vec3(0.0f, 0.0f, 1.0f), rate * dt);
}
