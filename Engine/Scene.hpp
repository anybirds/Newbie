#pragma once

#include <string>
#include <set>
#include <unordered_set>

#include <Asset.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Drawer.hpp>

NAMESPACE(Engine) {

    class Group;
    class GameObject;
    class Script;

    CLASS_FINAL_ATTR(SceneSetting, Entity, ENGINE_EXPORT) {
        TYPE(SceneSetting);
    };

    /*
    Abstraction of a scene that has multiple Groups of GameObjects.
    */
    class ENGINE_EXPORT Scene final {
    public:
        static Scene &GetInstance() { static Scene scene; return scene; }

    private:
        Scene() {}

        bool loaded;
        std::string name;
        std::string path;
        SceneSetting *setting;
        std::unordered_set<Group *> groups;
        std::unordered_set<Script *> scripts;
        std::multiset<Renderer *, RendererComparer> renderers;
        std::multiset<Drawer *, DrawerComparer> drawers;

        std::unordered_set<Group *> garbages;
        
    public:
        Scene(const Scene &) = delete;
        void operator=(const Scene &) = delete;

        bool Load(const std::string &path);
        bool Save();
        void Close();

        bool IsLoaded() const { return loaded; }
        const std::string &GetName() const { return name; }
        const std::string &GetPath() const { return path; }
        SceneSetting *GetSetting() const { return setting; }
        
        const std::unordered_set<Group *> &GetAllGroups() const { return groups; }
        Group *AddGroup();
        void RemoveGroup(Group *group);

        GameObject *FindGameObject(const std::string &name);
        
        
        void Start();
        void Refresh();
        void Update();
        void Render();
        void Delete();

        friend class Group;
        friend class GameObject;
        friend class Camera;
        friend class Script;
        friend class Renderer;
        friend class Drawer;
    };
}