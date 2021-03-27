#pragma once

#include <string>
#include <unordered_set>

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
        static bool Load(int index);
        static bool Save();
        static void Close();

    private:
        Scene() {}

        std::string name;
        std::string path;
        SceneSetting *setting;
        std::unordered_set<Group *> groups;

        std::unordered_set<Group *> garbages;
        
    public:
        Scene(const Scene &) = delete;
        void operator=(const Scene &) = delete;

        Group *AddGroup();
        void RemoveGroup(Group *group);
        GameObject *FindGameObject(const std::string &name);
        SceneSetting *GetSettiing() { return setting; }
        
        void Start();
        void Refresh();
        void Update();
        void Render();
        void Delete();

        friend class Group;
    };
}