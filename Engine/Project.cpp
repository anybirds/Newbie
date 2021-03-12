#include <iostream>
#include <fstream>

#include <Project.hpp>
#include <Scene.hpp>
#include <Type.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

bool Project::Load(const string &name) {
    Project &project = GetInstance();

    // close project
    Project::Close();

    // load shared library
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    project.lib = LoadLibrary((name + ".dll").c_str());
    if (!project.lib) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot load project library: " << name << '\n';
        return false;
    }
    project.init = reinterpret_cast<func>(GetProcAddress(project.lib, "type_init"));
    project.clear = reinterpret_cast<func>(GetProcAddress(project.lib, "type_clear"));
#else
    project.lib = dlopen((name + ".so").c_str(), RTLD_LAZY);
    if (!project.lib) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot load project library: " << name << '\n';
        return false;
    }
    project.init = reinterpret_cast<func>(dlsym(project.lib, "type_init"));
    project.clear = reinterpret_cast<func>(dlsym(project.lib, "type_clear"));
#endif    
    if (!project.init) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot resolve type_init function symbol: " << name << '\n';
        return false;
    }
    if (!project.clear) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot resolve type_clear function symbol: " << name << '\n';
        return false;
    }
    project.init();

    // open json file
    ifstream fs(name);
    if (fs.fail()) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot open project: " << name << '\n';
        return false;
    }

    try {
        // read json object
        json js;
        fs >> js;
        
        // read project setting
        ProjectSetting::type->Deserialize(js[ProjectSetting::type->GetName()], project.setting);
        
        // read scenes
        project.scenes = js["Scene"];

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
        Project::Close();
        cerr << '[' << __FUNCTION__ << ']' << " cannot read project: " << name << '\n';
        return false;
    }
    
    cerr << '[' << __FUNCTION__ << ']' << " read project: " << name << " done.\n";
    return true;
}

bool Project::Save() {
    Project &project = Project::GetInstance();

    // save scene
    if (!Scene::Save()) {
        return false;
    }

    // open json file
    ofstream fs(project.name);
    if (fs.fail()) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot open project: " << project.name << '\n';
        return false;
    }

    try {
        json js;
        
        // write project setting
        ProjectSetting::type->Serialize(js[ProjectSetting::type->GetName()], project.setting);

        // write scenes
        js["Scene"] = project.scenes;

        // write assets
        json &assets = js["Asset"];
        unordered_map<uint64_t, Asset *> temp;
        for (auto it = project.assets.begin(); it != project.assets.end(); ) {
            if (it->second->IsRemoved()) {
                temp.insert(*it);
                it = project.assets.erase(it);
            } else {
                Type *type = it->second->GetType();
                type->Serialize(assets[type->GetName()][to_string(it->first)], it->second);
                it++;
            }
        }
        project.assets.insert(temp.begin(), temp.end());

        fs << js;
    } catch(...) {
        Project::Close();
        cerr << '[' << __FUNCTION__ << ']' << " cannot save project: " << project.name << '\n';
        return false;
    }

    cerr << '[' << __FUNCTION__ << ']' << " save project: " << project.name << "done.\n";
    return true;
}

void Project::Close() {
    // close scene
    Scene::Close();

    // clear out members
    Project &project = GetInstance();
    project.name.clear();
    delete project.setting;
    project.scenes.clear();
    for (auto &p : project.assets) {
        delete p.second;
    }
    project.assets.clear(); 

    // clear out lib and types
    project.clear();
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    FreeLibrary((HMODULE)project.lib);
#else
    dlclose(project.lib);
#endif
}

const string &Project::GetScene(const string &name) const {
    return scenes.at(name).get<string>();
}

void Project::AddScene(const string &name, const string &path) {
    scenes[name] = path;
}

void Project::RemoveScene(const string &name) {
    scenes.erase(name);
}