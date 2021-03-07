#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include <EngineExport.hpp>
#include <Entity.hpp>
#include <Asset.hpp>

namespace Engine {

    class Type;
    class AScene;

    class ENGINE_EXPORT ProjectSetting final : public Entity {
    public:
        static Type *type;
        
    private:
        uint64_t serial;
        std::string startSceneName;
        
    public:
        uint64_t GetSerial() { return serial++; }
        const std::string &GetStartSceneName() const { return startSceneName; }
        void SetStartSceneName(const std::string &name) { startSceneName = name; }
        
        friend class Project;
    };
    
    /*
    Abstraction of a user project that includes User module, used Resources, and Scenes.
    */
    class ENGINE_EXPORT Project final {
    public:
        static Project &GetInstance() { static Project project; return project; }
        static bool Load(const std::string &name);
        static bool Save();
        static void Clear();
        
    private:
        Project() {}
        
        std::string name;
        ProjectSetting *setting;
        std::unordered_map<std::string, AScene *> scenes;
        std::unordered_map<uint64_t, Asset *> assets;

    public:
        Project(const Project &) = delete;
        void operator=(const Project &) = delete;
        
        void AddScene(const std::string &name, const std::string &path);
        void RemoveScene(const std::string &name);
        template <class T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool> = true>
        T *AddAsset() {
            T *asset = new T();
            asset->serial = setting->GetSerial();
            assets.insert({asset->serial, asset});
            return asset;
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool> = true>
        void RemoveAsset(uint64_t serial) {
            assets.erase(serial);
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool> = true>
        T *GetAsset(uint64_t serial) {
            auto it = assets.find(serial);
            if (it != assets.end()) {
                return dynamic_cast<T *>(*it);
            } else {
                return nullptr;
            }
        }

        friend class Scene;
    };
}
/*
#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <unordered_set>

#include <engine_global.hpp>

namespace Engine {

    class Scene;
    class Resource;

    class ENGINE_EXPORT Project final {
    private:
        static std::string path;
        static std::unordered_set<Scene *> sceneset;
        static std::unordered_set<Resource *> resset;

    public:
        static void Clear();
        static bool Load(const std::string &path);
        static void Save();

        friend class Scene;
        friend class Resource;
        friend class ProjectWidget;
    };
}
#endif
*/
