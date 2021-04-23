#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

#include <nlohmann/json.hpp>

#include <Project.hpp>
#include <Scene.hpp>


using json = nlohmann::json;
using namespace std;
using namespace Engine;

bool Project::Load(const string &path) {
    // close project
    Close();

    // resolve name, directory
    auto fspath = filesystem::absolute(filesystem::u8path(path));
    this->path = fspath.u8string();
    name = fspath.filename().stem().u8string();
    directory = fspath.parent_path().u8string();

    // compile library
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    wstring gcmd;
    wstring wdir = filesystem::u8path(directory).wstring();
    wstring nbpath(filesystem::u8path(NEWBIE_PATH).wstring());
    if (_MSC_VER >= 1920) {
        gcmd = L"cmake -G \"Visual Studio 16 2019\" -A x64 -B \"" + wdir + L"/build\" \"" + wdir + L"\" -DNEWBIE_PATH=\"" + nbpath + L"\"";
    } else if (_MSC_VER >= 1910) {
        gcmd = L"cmake -G \"Visual Studio 15 2017\" -A x64 -B \"" + wdir + L"/build\" \"" + wdir + L"\" -DNEWBIE_PATH=\"" + nbpath + L"\"";
    } else {
        cerr << '[' << __FUNCTION__ << ']' << " inappropriate Visual Studio version: " << _MSC_VER << '\n';
        return false;
    }
    _wsystem(gcmd.c_str());
    wstring bcmd(L"cmake --build \"" + wdir + L"/build\" --config Release");
    _wsystem(bcmd.c_str());
#else
    string nbpath(NEWBIE_PATH);
    string gcmd("cmake -G \"Unix Makefiles\" -B \"" + directory + "/build\" \"" + directory + "\" -DNEWBIE_PATH=\"" + nbpath + "\"");
    system(gcmd.c_str());
    string bcmd("cmake --build \"" + directory + "/build\"");
    system(bcmd.c_str());
#endif
    
    // load shared library
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    libpath = filesystem::u8path(directory).wstring() + L"/build/Release/User.dll";
    lib = LoadLibraryW((libpath).c_str());
    if (!lib) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot load project library\n";
        return false;
    }
    init = reinterpret_cast<func>(GetProcAddress(lib, "type_init"));
    clear = reinterpret_cast<func>(GetProcAddress(lib, "type_clear"));
#else
    libpath = directory + "build/User.dll";
    lib = dlopen((name + ".so").c_str(), RTLD_LAZY);
    if (!lib) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot load project library: " << libpath << '\n';
        return false;
    }
    init = reinterpret_cast<func>(dlsym(lib, "type_init"));
    clear = reinterpret_cast<func>(dlsym(lib, "type_clear"));
#endif    
    if (!init) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot resolve type_init function symbol\n";
        return false;
    }
    if (!clear) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot resolve type_clear function symbol\n";
        return false;
    }

    try {
        init();
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " type_init failed: " << name << '\n';
        return false;
    }
    cerr << '[' << __FUNCTION__ << ']' << " loading shared library done.\n";

    // open json file
    ifstream fs(fspath);
    if (fs.fail()) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot open project: " << this->path << '\n';
        return false;
    }

    try {
        // read json object
        json js;
        fs >> js;
        
        // read scenes
        scenes = js["scenes"].get<unordered_set<string>>();
        
        // read assets
        json &assets = js["assets"];
        for (json::iterator i = assets.begin(); i != assets.end(); i++) {
            const Type *type = Type::GetType(i.key());
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                Entity *entity = type->Instantiate();
                this->assets.insert({stoll(j.key()), dynamic_cast<Asset *>(entity)});
            }
        }
        
        for (json::iterator i = assets.begin(); i != assets.end(); i++) {
            const Type *type = Type::GetType(i.key());
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                type->Deserialize(j.value(), this->assets.at(stoll(j.key())));
            }
        }
        
        // read project setting
        setting = (ProjectSetting *)ProjectSetting::StaticType()->Instantiate();
        ProjectSetting::StaticType()->Deserialize(js["setting"], setting);

    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot read project: " << name << '\n';
        Close();
        return false;
    }

    loaded = true;
    cerr << '[' << __FUNCTION__ << ']' << " read project: " << name << " done.\n";
    return true;
}

bool Project::Save() {
    // save scene
    Scene &scene = Scene::GetInstance();
    if (scene.IsLoaded() && !scene.Save()) {
        return false;
    }

    try {
        json js;
        
        // write project setting
        ProjectSetting::StaticType()->Serialize(js["setting"], setting);

        // write scenes
        js["scenes"] = scenes;

        // write assets
        json &assets = js["assets"];
        for (auto &p : this->assets) {
            // ignore removed assets
            if (p.second->IsRemoved()) {
                continue;
            }
        
            Type *type = p.second->GetType();
            type->Serialize(assets[type->GetName()][to_string(p.first)], p.second);
        }

        // open json file
        ofstream fs(filesystem::u8path(path));
        if (fs.fail()) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot open project: " << path << '\n';
            return false;
        }
        fs << js;
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot save project: " << name << '\n';
        Close();
        return false;
    }

    cerr << '[' << __FUNCTION__ << ']' << " save project: " << name << " done.\n";
    return true;
}

void Project::Close() {
    // close scene
    Scene &scene = Scene::GetInstance();
    scene.Close();
    Scene &backup = Scene::GetBackup();
    backup.Close();

    // clear out members
    if (setting) {
        delete setting;
    }
    for (auto &p : assets) {
        delete p.second;
    }

    // clear out lib and types
    if (lib) {
        clear();
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        FreeLibrary((HMODULE)lib);
#else
        dlclose(lib);
#endif
    }  

    *this = Project();

    cerr << '[' << __FUNCTION__ << ']' << " close project done.\n";
}