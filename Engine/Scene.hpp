#pragma once

#include <cstdint>
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
    private:
        static void ToBackup();
        static void FromBackup();

    public:
        static Scene &GetInstance() { static Scene scene; return scene; }
        static Scene &GetBackup() { static Scene backup; return backup; }
 
    private:
        Scene() : flags(0U), loaded(false), setting(nullptr) {}
        Scene &operator=(const Scene &) = default;

        enum {
            LOAD = 1,
            SAVE = 1 << 1,
            CLOSE = 1 << 2,
        };
        uint8_t flags;
        std::string loadPath;

        bool loaded;
        std::string name;
        std::string path;
        SceneSetting *setting;
        std::unordered_set<GameObject *> roots;
        std::unordered_set<Script *> scripts;
        std::map<int, std::set<Renderer *>> renderers;
        std::map<int, std::map<BatchKey, Batch *>> batches;
        
        std::vector<Component *> adds;
        std::vector<Component *> removes;
        std::vector<Component *> enables;
        std::vector<Component *> disables;

        void Flags();
        
        void Enable();
        void Disable();
        void Add();
        void Remove();

        void Start();
        void Update();
        void Render();

    public:
        Scene(const Scene &) = delete;

        bool Load(const std::string &path);
        bool Save();
        void Close();
        bool LoadImmediate(const std::string &path);
        bool SaveImmediate();
        void CloseImmediate();

        bool IsLoaded() const { return loaded; }
        const std::string &GetName() const { return name; }
        const std::string &GetPath() const { return path; }
        SceneSetting *GetSetting() const { return setting; }
        
        const std::map<int, std::map<BatchKey, Batch *>> &GetAllBatches() const { return batches; }
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
        friend class Material;
    };
}