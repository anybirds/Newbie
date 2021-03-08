#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <Entity.hpp>
#include <Type.hpp>

namespace Engine {

    class GameObject;
    class Renderer;
    class Script;

    class ENGINE_EXPORT Group final : public Entity {
        TYPE(Group)

    private:
        std::unordered_map<std::string, GameObject *> gameObjects;
        std::vector<GameObject *> garbages;
        std::vector<Renderer *> renderers;
        std::vector<Script *> scripts;

    public:
        ~Group();

        GameObject *GetGameObject(const std::string &name) const;
        GameObject *AddGameObject(const std::string &name);
        bool RemoveGameObject(const std::string &name);

        friend class GameObject;
        friend class Scene;
    };
}