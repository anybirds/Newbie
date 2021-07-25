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

    PROPERTY_NONE(uint64_t, serial);
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

public:
    Project(const Project &) = delete;
    
    bool Load(const std::string &path);
    bool Save();
    void Close();

    bool IsLoaded() const { return loaded; }
    const std::string &GetName() const { return name; }
    const std::string &GetDirectoy() const { return directory; }
    ProjectSetting *GetSetting() const { return setting; }

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
        for (auto &p : GetAllAssets()) {
            if (Type::IsBaseOf(type, p.second->GetType())) { ret.push_back(p.second); }
        }
        return ret;
    }

    template <class T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool> = true>
    T *AddAsset() {
        T *asset = new T();
        asset->serial = setting->GetSerial();
        assets.insert({asset->serial, asset});
        return asset;
    }

    void RemoveAsset(Asset *asset) { assets.erase(asset->GetSerial()); }
    void DestroyAsset(Asset *asset) { assets.erase(asset->GetSerial()); delete asset; }

    friend class Scene;
};