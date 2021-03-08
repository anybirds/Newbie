#include <GameObject.hpp>
#include <Component.hpp>

using namespace std;
using namespace Engine;

GameObject::~GameObject() {
    for (Component *component : components) {
        delete component;
    }
}