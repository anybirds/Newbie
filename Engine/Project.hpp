#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

#include <Entity.hpp>
#include <Asset.hpp>

CLASS_FINAL_ATTR(ProjectSetting, Entity, ENGINE_EXPORT) {
    TYPE(ProjectSetting);

    PROPERTY_NONE(uint64_t, serial, Serial);
    PROPERTY(std::string, startScene, StartScene);
    
public:
    uint64_t GetSerial() { return ++serial; }
    
    friend class Project;
};

class ENGINE_EXPORT Project final {
public:
    static Project &GetInstance() { static Project project; return project; }
    
private:
    Project() : loaded(false), lib(nullptr), init(nullptr), clear(nullptr), setting(nullptr) {}
    Project &operator=(const Project &) = default;

    void WriteBlueprints(nlohmann::json &blueprints);

    bool loaded;
    typedef void (*func)();
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    HINSTANCE lib;
#else
    void *lib;
#endif
    func init;
    func clear;
    std::string path;
    std::string name;
    std::string directory;
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    std::wstring libpath;
#else
    std::string libpath;
#endif
    ProjectSetting *setting;
    std::unordered_set<std::string> scenes;
    std::unordered_map<uint64_t, Asset *> assets;
    nlohmann::json blueprints;

public:
    Project(const Project &) = delete;
    
    bool Load(const std::string &path);
    bool Save();
    void Close();

    bool IsLoaded() const { return loaded; }
    const std::string &GetName() const { return name; }
    const std::string &GetPath() const { return path; }
    const std::string &GetDirectoy() const { return directory; }
    ProjectSetting *GetSetting() const { return setting; }
    nlohmann::json &GetClassBlueprints() {  return blueprints; }

    const std::unordered_set<std::string> &GetAllScenes() const { return scenes; }
    void AddScene(const std::string &path) { scenes.insert(path); }
    void RemoveScene(const std::string &path) { scenes.erase(path); }

    const std::unordered_map<uint64_t, Asset *> &GetAllAssets() const { return assets; }

    Asset *GetAsset(uint64_t serial) const {
        auto it = assets.find(serial);
        if (it != assets.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    template <class T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool> = true>
    std::vector<T *> GetAssets() const {
        std::vector<T *> ret;
        for (auto &p : GetAllAssets()) {
            if (Type::IsBaseOf(T::StaticType(), p.second->GetType())) { ret.push_back((T *)p.second); }
        }
        return ret;
    }
    std::vector<Asset *> GetAssets(Type *type) const {
        std::vector<Asset *> ret;
        if (!Type::IsBaseOf(Asset::StaticType(), type)) {
            return ret;
        }
        for (auto &p : GetAllAssets()) {
            if (Type::IsBaseOf(type, p.second->GetType())) { ret.push_back(p.second); }
        }
        return ret;
    }

    template <class T, std::enable_if_t<std::is_base_of_v<Asset, T> && !std::is_abstract_v<T>, bool> = true>
    T *AddAsset() {
        T *t = new T();
        t->serial = setting->GetSerial();
        assets.insert({t->serial, t});
        return t;
    }
    Asset *AddAsset(Type *type) {
        if (!Type::IsBaseOf(Asset::StaticType(), type) || type->IsAbstract()) {
            return nullptr;
        }
        Asset *asset = (Asset *)type->Instantiate();
        asset->serial = setting->GetSerial();
        assets.insert({asset->serial, asset});
        return asset;
    }
    template <class T, std::enable_if_t<std::is_base_of_v<Asset, T> && !std::is_abstract_v<T>, bool> = true>
    T *AddAsset(T *asset) {
        nlohmann::json js;
        T::Serialize(js, asset);
        T *t = new T();
        T::Deserialize(js, t);
        t->serial = setting->GetSerial();
        assets.insert({t->serial, t});
        return t;
    }
    Asset *AddAsset(Type *type, Asset *asset) {
        if (!Type::IsBaseOf(Asset::StaticType(), type) || type->IsAbstract()) {
            return nullptr;
        }
        nlohmann::json js;
        type->Serialize(js, asset);
        Asset *ret = (Asset *)type->Instantiate();
        type->Deserialize(js, ret);
        ret->serial = setting->GetSerial();
        assets.insert({ret->serial, ret});
        return ret;
    }
    template <class T, std::enable_if_t<std::is_base_of_v<Asset, T> && !std::is_abstract_v<T>, bool> = true>
    T *AddAsset(const nlohmann::json &js) {
        T *t = new T();
        T::Deserialize(js, t);
        t->serial = setting->GetSerial();
        assets.insert({t->serial, t});
        return t;
    }
    Asset *AddAsset(Type *type, const nlohmann::json &js) {
        if (!Type::IsBaseOf(Asset::StaticType(), type) || type->IsAbstract()) {
            return nullptr;
        }
        Asset *asset = (Asset *)type->Instantiate();
        type->Deserialize(js, asset);
        asset->serial = setting->GetSerial();
        assets.insert({asset->serial, asset});
        return asset;
    }

    friend class Scene;
    friend class Asset;
};