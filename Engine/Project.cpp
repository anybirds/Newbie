#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include <iostream>
#include <fstream>
#include <unordered_map>

#include <Project.hpp>
#include <Resource.hpp>
#include <Scene.hpp>
#include <Type.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

bool Project::Load(const string &name) {
    // load shared library
    

    // open json file
    ifstream pfs(name);
    if (pfs.fail()) {
        return false;
    }

    try {
        // clear project
        Project::Clear();

        // read json object
        json js;
        pfs >> js;

        Project &project = GetInstance();
        
        // read project setting
        ProjectSetting::type->Deserialize(js["ProjectSetting"], project.setting);
        
        // read scenes
        json &scenes = js["Scene"];
        for (json::iterator i = scenes.begin(); i != scenes.end(); i++) {
            AScene *ascene = new AScene();
            AScene::type->Deserialize(*i, ascene);
            project.scenes.insert({ascene->name, ascene});
        }

        // read assets
        json &assets = js["Asset"];
        for (json::iterator i = assets.begin(); i != assets.end(); i++) {
            const Type *type = Type::GetType(i.key());
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                Entity *entity = type->Instantiate();
                project.assets.insert({stoll(j.key()), dynamic_cast<Asset *>(entity)});
            }
        }
        for (json::iterator i = assets.begin(); i != assets.end(); i++) {
            const Type *type = Type::GetType(i.key());
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                type->Deserialize(j.value(), project.assets.at(stoll(j.key())));
            }
        }
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot read project file: " << name << '\n';

        Project::Clear();
        return false;
    }
    
    cerr << '[' << __FUNCTION__ << ']' << " read project file: " << name << " done.\n";
    return true;
}

bool Project::Save() {
    Project &project = Project::GetInstance();

    // open json file
    ofstream pfs(project.name);
    if (pfs.fail()) {
        return false;
    }

    try {
        json js;
        
        // write project setting
        ProjectSetting::type->Serialize(js["ProjectSetting"], project.setting);

        // write scenes
        json &scenes = js["Scene"];
        for (auto &p : project.scenes) {
            AScene::type->Serialize(scenes[p.first], p.second);
        }

        // write assets
        json &assets = js["Asset"];
        // think about how to get the serialize function from the object.
        
        pfs << js;
    } catch(...) {
        return false;
    }

    return true;
}

void Project::Clear() {
    Scene::Clear();
    Project &project = GetInstance();
    project.name.clear();
    delete project.setting;
    for (auto &p : project.scenes) {
        delete p.second;
    }
    project.scenes.clear();
    for (auto &p : project.assets) {
        delete p.second;
    }
    project.assets.clear(); 
}