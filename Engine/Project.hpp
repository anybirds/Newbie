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
#include <nlohmann/json.hpp>

#include <Entity.hpp>
#include <Asset.hpp>
#include <Type.hpp>

namespace Engine {

    class ENGINE_EXPORT [[Serialize]] ProjectSetting final : public Entity {
        TYPE(ProjectSetting)

    private:
        uint64_t serial;
        std::string startSceneName;
        
    public:
        uint64_t GetSerial() { return ++serial; }
        const std::string &GetStartSceneName() const { return startSceneName; }
        void SetStartSceneName(const std::string &name) { startSceneName = name; }
        
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
        nlohmann::json scenes;
        std::unordered_map<uint64_t, Asset *> assets;

    public:
        Project(const Project &) = delete;
        void operator=(const Project &) = delete;
        
        const std::string &GetName() const { return name; }
        const std::string &GetDirectoy() const { return directory; }
        ProjectSetting *GetSetting() const { return setting; }
        const std::string &GetScene(const std::string &name) const;
        void AddScene(const std::string &name, const std::string &path);
        void RemoveScene(const std::string &name);
        template <class T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool> = true>
        T *AddAsset(const std::string &name) {
            T *asset = new T();
            asset->SetName(name);
            assets.insert({setting->GetSerial(), asset});
            return asset;
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool> = true>
        T *GetAsset(uint64_t serial) const {
            auto it = assets.find(serial);
            if (it != assets.end() && !(it->second->IsRemoved())) {
                return dynamic_cast<T *>(it->second);
            } else {
                return nullptr;
            }
        }

        friend class Scene;
    };
}