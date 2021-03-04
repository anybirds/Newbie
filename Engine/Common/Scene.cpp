#include <fstream>
#include <GL/glew.h>

#include <nlohmann/json.hpp>

#include <Common/Component.hpp>
#include <Common/Debug.hpp>
#include <Common/GameObject.hpp>
#include <Common/Project.hpp>
#include <Common/Resource.hpp>
#include <Common/Scene.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Model.hpp>

using namespace std;
using json = nlohmann::json;
using namespace Engine;

namespace Engine {
    TYPE_DEF(SceneSetting)
    SER_DEF(SceneSetting, Object,
    MEMBER_SER | MEMBER_SHOW, Camera *, mainCamera
    )

    TYPE_DEF(Scene)
    SER_DEF(Scene, Object,
    MEMBER_SER | MEMBER_SHOW, std::string, path
    )
}

SceneSetting::SceneSetting(const std::string &name, Type *type) : Object(name, type) {
    Scene::curr->settingset.insert(this);
}

SceneSetting::~SceneSetting() {
    Scene::curr->settingset.erase(this);
}

Scene *Scene::curr;

Scene::Scene(const std::string &name, Type *type) : Object(name, type) {
    // glew init
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK) {
        // Initializing GLEW failed
        cout << '[' << __FUNCTION__ << ']' << " message: " << glewGetErrorString(glew_error) << '\n';
        exit(1);
    }

    Project::sceneset.insert(this);
}

Scene::~Scene() {
    Project::sceneset.erase(this);
}

void Scene::Clear() {
    if (!IsValid(curr)) {
        return;
    }

    unordered_set<SceneSetting *> settingset(curr->settingset);
    unordered_set<GameObject *> goset(curr->goset);
    unordered_set<Component *> compset(curr->compset);

    for (SceneSetting *setting : settingset) {
        delete setting;
    }
    for (GameObject *go : goset) {
        delete go;
    }
    for (Component *comp : compset) {
        delete comp;
    }
    curr = nullptr;
}

bool Scene::Load(const string &name) {
    Scene *prev = curr;

    Scene *scene = Find<Scene>(name);
    curr = scene;

    if (!IsValid(scene)) {
        curr = prev;
        return false;
    }

    // read json file
    ifstream sfs(scene->GetPath());
    if (sfs.fail()) {
        curr = prev;
        return false;
    }

    try {
        json js;
        sfs >> js;

#ifdef DEBUG
        cout << '[' << __FUNCTION__ << ']' << " read scene file: " << scene->GetPath() << " done ..." << endl;
#endif

        // pre-deserialization
        for (json::iterator i = js.begin(); i != js.end(); i++) {
            Type *type = Type::Find(i.key());
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                type->Instantiate(j.key());
            }
        }

#ifdef DEBUG
        cout << '[' << __FUNCTION__ << ']' << " pre-deserialization done ..." << endl;
#endif

        // deserialization
        // mark all the resources used by makring shouldLoad to true
        for (Resource *res : Project::resset) {
            res->shouldLoad = false;
        }

        Resource::sceneLoad = true;
        for (json::iterator i = js.begin(); i != js.end(); i++) {
            Type *type = Type::Find(i.key());
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                type->Deserialize(j.value(), Find<Object>(j.key()));
            }
        }
        Resource::sceneLoad = false;

#ifdef DEBUG
        cout << '[' << __FUNCTION__ << ']' << " deserialization done ..." << endl;
#endif

         for (Resource *res : Project::resset) {
             if (!res->loaded && res->shouldLoad) {
                 // mark shouldLoad to true recursively (there should be no cyclic dependencies between resources)
                 res->OnInit();
             }
         }
         // remove all resources that are not in use (including Model, Shader ...)
         for (Resource *res : Project::resset) {
             if (res->loaded && !res->shouldLoad) {
                 res->OnDestroy();
             }
         }

#ifdef DEBUG
        cout << '[' << __FUNCTION__ << ']' << " loading resource done ..." << endl;
#endif

        for (Component *comp : scene->compset) {
            comp->OnInit();
        }
        for (GameObject *go : scene->goset) {
            go->OnInit();
        }
        for (SceneSetting *setting : scene->settingset) {
            setting->OnInit();
        }

#ifdef DEBUG
        cout << '[' << __FUNCTION__ << ']' << " post-deserialization done ..." << endl;
#endif
    } catch (...) {
        curr = prev;
        return false;
    }


    return true;
}

void Scene::Save() {
    if (!IsValid(Scene::curr)) {
        return;
    }

    ofstream sfs(Scene::curr->path);
    json js;

    for (SceneSetting *setting : Scene::curr->settingset) {
        js["SceneSetting"][setting->GetName()] = *setting;
    }

    for (GameObject *go : Scene::curr->goset) {
        js["GameObject"][go->GetName()] = *go;
    }

    for (Component *comp : Scene::curr->compset) {
        Type *type = GetType(comp);
        type->Serialize(js[type->GetName()][comp->GetName()], comp);
    }

    sfs << js.dump(4);
}
