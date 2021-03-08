#pragma once

#include <string>
#include <unordered_set>

#include <EngineExport.hpp>
#include <Entity.hpp>
#include <Asset.hpp>
#include <Type.hpp>

namespace Engine {

    class Camera;
    class Group;

    class ENGINE_EXPORT SceneSetting final : public Entity {
        TYPE(SceneSetting)

    private:
        Camera *mainCamera;
        
    public:
        Camera *GetMainCamera() const { return mainCamera; }
        void SetMainCamera(Camera *camera) { mainCamera = camera; }
    };

    /*
    Abstraction of a scene that has multiple Groups of GameObjects.
    */
    class ENGINE_EXPORT Scene final {
    public:
        static Scene &GetInstance() { static Scene scene; return scene; }  
        static bool Load(const std::string &name);
        static bool Save();
        static void Close();

    private:
        Scene() {}

        std::string name;
        SceneSetting *setting;
        std::unordered_set<Group *> groups;
        
    public:
        Scene(const Scene &) = delete;
        void operator=(const Scene &) = delete;

        Group *AddGroup();
        void RemoveGroup(Group *group);
    };
}

/*
#ifndef SCENE_H
#define SCENE_H

#include <unordered_set>

#include <Common/Object.hpp>

#include <engine_global.hpp>

namespace Engine {

    class GameObject;
    class Component;
    class Camera;

    SER_DECL(ENGINE_EXPORT, SceneSetting)

    class ENGINE_EXPORT SceneSetting final : public Object {
        TYPE_DECL(SceneSetting)

        PROPERTY(Camera *, MainCamera, mainCamera)

    public:
        SceneSetting(const std::string &name, Type *type = SceneSetting::type);
        virtual ~SceneSetting();
    };

    SER_DECL(ENGINE_EXPORT, Scene)

    class ENGINE_EXPORT Scene final : public Object {
        TYPE_DECL(Scene)

        PROPERTY(std::string, Path, path)

    private:
        static Scene *curr;

    public:
        static void Clear();
        static bool Load(const std::string &name);
        static void Save();

        static Scene *GetCurrentScene() { return curr; }

    private:
        std::unordered_set<SceneSetting *> settingset;
        std::unordered_set<GameObject *> goset;
        std::unordered_set<Component *> compset;

    public:
        Scene(const std::string &name, Type *type = Scene::type);
        virtual ~Scene() override;

        friend class SceneSetting;
        friend class GameObject;
        friend class Component;

        friend class HierarchyWidget;
    };
}

typedef typename concat<TYPE_LIST, Engine::SceneSetting>::type TypeListSceneSetting;
#undef TYPE_LIST
#define TYPE_LIST TypeListSceneSetting

typedef typename concat<TYPE_LIST, Engine::Scene>::type TypeListScene;
#undef TYPE_LIST
#define TYPE_LIST TypeListScene

#endif
*/