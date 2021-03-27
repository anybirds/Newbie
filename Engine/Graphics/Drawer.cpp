#include <Group.hpp>
#include <GameObject.hpp>
#include <Graphics/Drawer.hpp>

using namespace Engine;

void Drawer::SetOrder(unsigned order) {
    Group *group = GetGroup();
    group->drawers.erase(this);
    this->order = order;
    group->drawers.insert(this);
}