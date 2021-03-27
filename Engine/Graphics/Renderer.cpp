#include <Group.hpp>
#include <GameObject.hpp>
#include <Graphics/Renderer.hpp>

using namespace Engine;

void Renderer::SetOrder(unsigned order) {
    Group *group = GetGroup();
    group->renderers.erase(this);
    this->order = order;
    group->renderers.insert(this);
}