#pragma once

#include <string>
#include <list>

#include <Entity.hpp>
#include <Asset.hpp>
#include <Type.hpp>

namespace Engine {

    class Camera;
    class Group;
    class GameObject;

    class ENGINE_EXPORT [[Serialize]] SceneSetting final : public Entity {
        TYPE(SceneSetting)

    private:
        Camera *mainCamera;
        
    public:
        Camera *GetMainCamera() const { return mainCamera; }
        void SetMainCamera(Camera *camera) { mainCamera = camera; }
    };

    /*
    Abstraction of a scene that has multiple Groups of GameObjects.
    */
    class ENGINE_EXPORT Scene final {
    public:
        static Scene &GetInstance() { static Scene scene; return scene; }  
        static bool Load(const std::string &name);
        static bool Save();
        static void Close();

    private:
        Scene() {}

        std::string name;
        SceneSetting *setting;
        std::list<Group *> groups;
        
    public:
        Scene(const Scene &) = delete;
        void operator=(const Scene &) = delete;

        Group *AddGroup();
        void RemoveGroup(Group *group);
        GameObject *GetGameObject(const std::string &name);
        
        void Start();
        void Update();
        void Render();

        friend class Group;
    };
}