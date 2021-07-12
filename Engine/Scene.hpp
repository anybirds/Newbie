#pragma once

#include <cstdint>
#include <string>
#include <set>
#include <unordered_set>
#include <map>
#include <vector>

#include <Entity.hpp>
#include <Graphics/Batch.hpp>

class GameObject;
class Transform;
class Component;
class Script;
class Renderer;
class Material;
class Mesh;

class ENGINE_EXPORT Scene final {
public:
    static Scene &GetInstance() { static Scene scene; return scene; }
    static Scene &GetBackup() { static Scene backup; return backup; }
    static void ToBackup();
    static void FromBackup();
    static void LoadBackup();

private:
    Scene() : flags(0U), loaded(false), root(nullptr) {}
    Scene &operator=(const Scene &) = default;

    enum {
        LOAD = 1,
        LOAD_ASYNC = 1 << 1,
    };
    uint8_t flags;
    std::string loadPath;

    bool loaded;
    std::string name;
    std::string path;
    GameObject *root;
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

    void Load(const std::string &path);
    bool LoadImmediate(const std::string &path);
    bool Save();
    void Close();

    bool IsLoaded() const { return loaded; }
    const std::string &GetName() const { return name; }
    const std::string &GetPath() const { return path; }
    void SetPath(const std::string &path) { this->path = path; }
    
    GameObject *GetRoot() const { return root; }
    const std::map<int, std::map<std::pair<Mesh *, Material *>, Batch *>> &GetAllBatches() const { return batches; }

    void Loop();
    void PauseLoop();

    friend class Transform;
    friend class Script;
    friend class Renderer;
    friend class Drawer;
    friend class Material;
};