#pragma once

#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <vector>

#include <Asset.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Batch.hpp>

NAMESPACE(Engine) {

    class GameObject;
    class Component;

    CLASS_FINAL_ATTR(SceneSetting, Entity, ENGINE_EXPORT) {
        TYPE(SceneSetting);
    };

    /*
    Abstraction of a scene that has multiple Groups of GameObjects.
    */
    class ENGINE_EXPORT Scene final {
    public:
        static Scene &GetInstance() { static Scene scene; return scene; }

        static Scene backup;
        static void SaveBackup();
        static void LoadBackup();
 
    private:
        Scene() {}

        bool loaded;
        std::string name;
        std::string path;
        SceneSetting *setting;
        std::unordered_set<GameObject *> roots;
        std::unordered_set<Script *> scripts;
        std::multiset<Renderer *, RendererComparer> renderers;
        std::map<BatchKey, Batch, BatchComparer> batches;
        
        std::vector<Component *> adds;
        std::vector<Component *> removes;
        std::vector<Component *> enables;
        std::vector<Component *> disables;

        void Enable();
        void Disable();
        void Add();
        void Remove();
        void Destroy();

        void Start();
        void Update();
        void Render();

    public:
        Scene(const Scene &) = delete;
        void operator=(const Scene &) = delete;

        bool Load(std::string path);
        bool Save();
        void Close();

        bool IsLoaded() const { return loaded; }
        const std::string &GetName() const { return name; }
        const std::string &GetPath() const { return path; }
        SceneSetting *GetSetting() const { return setting; }
        
        const std::unordered_set<GameObject *> &GetRootGameObjects() const { return roots; }
        GameObject *AddGameObject();
        GameObject *AddGameObject(GameObject *gameObject);
        void RemoveGameObject(GameObject *gameObject);
        void RemoveComponent(Component *component);
        GameObject *FindGameObject(const std::string &name);

        void PauseLoop();
        void PlayLoop();

        friend class Transform;
        friend class Script;
        friend class Renderer;
        friend class Drawer;
    };
}