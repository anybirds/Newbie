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
#include <nlohmann/json.hpp>

#include <Entity.hpp>
#include <Asset.hpp>
#include <Type.hpp>

NAMESPACE(Engine) {
    
    CLASS_FINAL_ATTR(ProjectSetting, Entity, ENGINE_EXPORT) {
        TYPE(ProjectSetting);

        PROPERTY_NONE(uint64_t, serial);
        PROPERTY(std::string, startScene, StartScene);
        
    public:
        uint64_t GetSerial() { return ++serial; }
        
        friend class Project;
    };
    
    /*
    Abstraction of a user project that includes User module, used Assets, and Scenes.
    */
    class ENGINE_EXPORT Project final {
    public:
        static Project &GetInstance() { static Project project; return project; }
        
    private:
        Project() {}

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

        std::unordered_set<Asset *> garbages;

    public:
        Project(const Project &) = delete;
        void operator=(const Project &) = delete;
        
        bool Load(const std::string &path);
        bool Save();
        void Close();

        bool IsLoaded() const { return loaded; }
        const std::string &GetName() const { return name; }
        const std::string &GetDirectoy() const { return directory; }
        ProjectSetting *GetSetting() const { return setting; }

        const std::unordered_set<std::string> &GetAllScenes() const { return scenes; }
        void AddScene(const std::string &path) { scenes.insert(path); }
        std::unordered_set<std::string>::iterator RemoveScene(std::unordered_set<std::string>::iterator it) {
            if (it == scenes.end()) {
                return it;
            }
            return scenes.erase(it);
        }
        void RemoveScene(const std::string &path) { scenes.erase(path); }

        const std::unordered_map<uint64_t, Asset *> &GetAllAssets() const { return assets; }
        template <class T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool> = true>
        T *GetAsset(uint64_t serial) const {
            auto it = assets.find(serial);
            if (it != assets.end()) {
                return dynamic_cast<T *>(it->second);
            } else {
                return nullptr;
            }
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool> = true>
        T *AddAsset() {
            T *asset = new T();
            asset->serial = setting->GetSerial();
            assets.insert({asset->serial, asset});
            return asset;
        }
        std::unordered_map<uint64_t, Asset *>::iterator RemoveAsset(std::unordered_map<uint64_t, Asset *>::iterator it);
        void RemoveAsset(Asset *asset);

        friend class Scene;
    };
}