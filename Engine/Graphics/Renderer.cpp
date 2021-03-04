#include <Graphics/Camera.hpp>
#include <Graphics/Material.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/Renderer.hpp>

using namespace std;
using namespace Engine;

namespace Engine {
    TYPE_DEF(Renderer)
    SER_DEF(Renderer, Component,
    MEMBER_SER | MEMBER_SHOW, Mesh *, mesh,
    MEMBER_SER | MEMBER_SHOW, Material *, material
    )
}

Renderer::Renderer(const string &name, Type *type) : Component(name, type) {
    Camera::rendset.insert(this);
}

Renderer::~Renderer() {
    Camera::rendset.erase(this);
}
