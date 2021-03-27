#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <nlohmann/json.hpp>

#include <Entity.hpp>
#include <Asset.hpp>
#include <Type.hpp>

namespace Engine {

    class ENGINE_EXPORT [[Serialize]] ProjectSetting final : public Entity {
        TYPE(ProjectSetting)

    private:
        uint64_t serial;
        int startSceneIndex;
        
    public:
        uint64_t GetSerial() { return ++serial; }
        int GetStartSceneIndex() const { return startSceneIndex; }
        void SetStartSceneIndex(int startScene) { this->startSceneIndex = startSceneIndex; }
        
        friend class Project;
    };
    
    /*
    Abstraction of a user project that includes User module, used Assets, and Scenes.
    */
    class ENGINE_EXPORT Project final {
    public:
        static Project &GetInstance() { static Project project; return project; }
        static bool Load(const std::string &path);
        static bool Save();
        static void Close();
        
    private:
        Project() {}

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
        std::string libpath;
        ProjectSetting *setting;
        std::vector<std::string> scenes;
        std::unordered_map<uint64_t, Asset *> assets;

        std::unordered_set<Asset *> garbages;

    public:
        Project(const Project &) = delete;
        void operator=(const Project &) = delete;
        
        const std::string &GetName() const { return name; }
        const std::string &GetDirectoy() const { return directory; }
        ProjectSetting *GetSetting() const { return setting; }
        const std::string &GetScene(int index) const;
        void AddScene(const std::string &path);
        void RemoveScene(int index);
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
        void RemoveAsset(Asset *asset);

        friend class Scene;
    };
}