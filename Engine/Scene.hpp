#pragma once

#include <cstdint>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <vector>

#include <Entity.hpp>
#include <Graphics/Batch.hpp>

NAMESPACE(Engine) {

    class GameObject;
    class Component;
    class Script;
    class Renderer;
    class Material;
    class Mesh;

    CLASS_FINAL_ATTR(SceneSetting, Entity, ENGINE_EXPORT) {
        TYPE(SceneSetting);
    };

    class ENGINE_EXPORT Scene final {
    public:
        static Scene &GetInstance() { static Scene scene; return scene; }
        static Scene &GetBackup() { static Scene backup; return backup; }
        static void ToBackup();
        static void FromBackup();

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
        std::map<int, std::unordered_set<Renderer *>> renderers;
        std::map<int, std::map<std::pair<Mesh *, Material *>, Batch *>> batches;
        
        std::vector<Component *> removes;
        std::vector<Component *> enables;
        std::vector<Component *> disables;

        void Flags();
        
        void Enable();
        void Disable();
        void Remove();

        void Update();

    public:
        Scene(const Scene &) = delete;

        void Load(const std::string &path);
        void Save();
        void Close();
        bool LoadImmediate(const std::string &path, bool useBackup = true);
        bool SaveImmediate();
        void CloseImmediate();

        bool IsLoaded() const { return loaded; }
        const std::string &GetName() const { return name; }
        const std::string &GetPath() const { return path; }
        void SetPath(const std::string &path) { this->path = path; }
        SceneSetting *GetSetting() const { return setting; }
        
        const std::map<int, std::map<std::pair<Mesh *, Material *>, Batch *>> &GetAllBatches() const { return batches; }
        const std::unordered_set<GameObject *> &GetRootGameObjects() const { return roots; }
        GameObject *AddGameObject();
        GameObject *AddGameObject(GameObject *gameObject);
        void RemoveGameObject(GameObject *gameObject);
        void RemoveComponent(Component *component);
        GameObject *FindGameObject(const std::string &name);

        void Render();
        void Loop();

        friend class Transform;
        friend class Script;
        friend class Renderer;
        friend class Drawer;
        friend class Material;
    };
}