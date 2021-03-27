#include <iostream>
#include <fstream>
#include <filesystem>

#include <Project.hpp>
#include <Scene.hpp>
#include <Type.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

bool Project::Load(const string &path) {

    Project &project = GetInstance();

    // close project
    Project::Close();

    // resolve name, directory
    project.path = filesystem::absolute(filesystem::path(path)).string();
    project.name = filesystem::path(project.path).filename().stem().string();
    project.directory = filesystem::path(project.path).parent_path().string();
    project.libpath = project.directory + "/User.dll";

    // load shared library
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    project.lib = LoadLibrary((project.libpath).c_str());
    if (!project.lib) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot load project library: " << project.libpath << '\n';
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
        cerr << '[' << __FUNCTION__ << ']' << " cannot resolve type_init function symbol: " << project.libpath << '\n';
        return false;
    }
    if (!project.clear) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot resolve type_clear function symbol: " << project.libpath << '\n';
        return false;
    }

    try {
        project.init();
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " type_init failed: " << project.name << '\n';
        return false;
    }
    cerr << '[' << __FUNCTION__ << ']' << " loading shared library: " << project.libpath << " done.\n";

    // open json file
    ifstream fs(project.path);
    if (fs.fail()) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot open project: " << project.path << '\n';
        return false;
    }

    try {
        // read json object
        json js;
        fs >> js;
        
        // read scenes
        project.scenes = js["Scene"].get<vector<string>>();
        
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
        
        // read project setting
        project.setting = (ProjectSetting *)ProjectSetting::StaticType()->Instantiate();
        ProjectSetting::StaticType()->Deserialize(js[ProjectSetting::StaticType()->GetName()], project.setting);

    } catch(...) {
        Project::Close();
        cerr << '[' << __FUNCTION__ << ']' << " cannot read project: " << project.name << '\n';
        return false;
    }
    
    cerr << '[' << __FUNCTION__ << ']' << " read project: " << project.name << " done.\n";
    return true;
}

bool Project::Save() {
    Project &project = Project::GetInstance();

    // save scene
    if (!Scene::Save()) {
        return false;
    }

    // open json file
    ofstream fs(project.path);
    if (fs.fail()) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot open project: " << project.path << '\n';
        return false;
    }

    try {
        json js;
        
        // write project setting
        ProjectSetting::StaticType()->Serialize(js[ProjectSetting::StaticType()->GetName()], project.setting);

        // write scenes
        js["Scene"] = project.scenes;

        // write assets
        json &assets = js["Asset"];
        for (auto &p : project.assets) {
            Type *type = p.second->GetType();
            type->Serialize(assets[type->GetName()][to_string(p.first)], p.second);
        }

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
    project.path.clear();
    project.directory.clear();
    project.name.clear();
    project.libpath.clear();
    if (project.setting) {
        delete project.setting;
    }
    project.scenes.clear();
    for (auto &p : project.assets) {
        delete p.second;
    }
    project.assets.clear(); 
    for (Asset *garbage : project.garbages) {
        delete garbage;
    }
    project.garbages.clear();

    // clear out lib and types
    if (project.lib) {
        project.clear();
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        FreeLibrary((HMODULE)project.lib);
#else
        dlclose(project.lib);
#endif
        project.lib = nullptr;
    }
    cerr << '[' << __FUNCTION__ << ']' << " close projece done.\n";
}

const string &Project::GetScene(int index) const {
    return scenes.at(index);
}

void Project::AddScene(const string &path) {
    scenes.push_back(path);
}

void Project::RemoveScene(int index) {
    scenes.at(index);
    scenes.erase(scenes.begin() + index);
}

void Project::RemoveAsset(Asset *asset) {
    assets.erase(asset->GetSerial());
    garbages.insert(asset);
}