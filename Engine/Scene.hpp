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
    class Transform;
    class Component;
    class Script;
    class Renderer;
    class Material;
    class Mesh;
    class Prefab;

    class ENGINE_EXPORT Scene final {
    public:
        static Scene &GetInstance() { static Scene scene; return scene; }
        static Scene &GetBackup() { static Scene backup; return backup; }
        static void ToBackup();
        static void FromBackup();
        static void LoadBackup();

    private:
        Scene() : flags(0U), loaded(false) {}
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
        std::vector<GameObject *> roots;
        std::unordered_set<Script *> scripts;
        std::map<int, std::unordered_set<Renderer *>> renderers;
        std::map<int, std::map<std::pair<Mesh *, Material *>, Batch *>> batches;
        
        std::vector<Component *> removeComps;
        std::vector<Transform *> removeTrans;
        std::vector<Component *> enables;
        std::vector<Component *> disables;

        void Flags();
        
        void Track();
        void Untrack();
        void Enable();
        void Disable();
        void Remove();
        void Destroy();

        void Render();
        void Update();

    public:
        Scene(const Scene &) = delete;

        void ToJson(nlohmann::json &js, bool nullify = false);
        void FromJson(const nlohmann::json &js, bool nullify = true);

        void Load(const std::string &path);
        void Save();
        void Close();
        bool LoadImmediate(const std::string &path);
        bool SaveImmediate();
        void CloseImmediate();

        bool IsLoaded() const { return loaded; }
        void SetLoaded(bool loaded) { this->loaded = loaded; }
        const std::string &GetName() const { return name; }
        const std::string &GetPath() const { return path; }
        void SetPath(const std::string &path) { this->path = path; }
        
        const std::map<int, std::map<std::pair<Mesh *, Material *>, Batch *>> &GetAllBatches() const { return batches; }
        const std::vector<GameObject *> &GetRootGameObjects() const { return roots; }
        GameObject *AddGameObject();
        GameObject *AddGameObject(GameObject *gameObject);
        GameObject *AddGameObject(const std::shared_ptr<Prefab> &prefab);
        void RemoveGameObject(GameObject *gameObject);
        void RemoveComponent(Component *component);
        GameObject *FindGameObject(const std::string &name);
        void DestroyGameObject(GameObject *gameObject);

        void Loop();
        void PauseLoop();

        friend class Transform;
        friend class Script;
        friend class Renderer;
        friend class Drawer;
        friend class Material;
    };
}