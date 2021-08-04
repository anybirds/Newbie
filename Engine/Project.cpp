#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <map>
#include <functional>

#include <nlohmann/json.hpp>

#include <Project.hpp>
#include <Scene.hpp>
#include <Prefab.hpp>

using namespace std;
using json = nlohmann::json;

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

        // read class blueprints
        unordered_map<string, map<pair<string, string>, int>> target;
        json &blueprints = js["blueprints"];
        for (json::iterator i = blueprints.begin(); i != blueprints.end(); i++) {
            auto &indexMap = target[i.key()];
            for (int j = 0; j < i.value()[1].size(); j++) {
                indexMap.insert({{i.value()[1][j][0].get<string>(), i.value()[1][j][1].get<string>()}, i.value()[0].get<int>() + j});
            }
        }

        // calculate difference
        unordered_map<string, vector<pair<bool, json>>> diff; // { <TypeName, <TargetExists, Default/Index> ... > ... }
        for (auto &p : Type::GetAllTypes()) {
            Type *type = p.second;
            auto &diffVector = diff[type->GetName()];
            
            // get base classes of the type
            vector<Type *> hierarchy; 
            Type *temp = type;
            while (temp) {
                hierarchy.push_back(temp);
                temp = temp->GetBase();
            }

            // put difference
            bool modified = false;
            int idx = 0;
            for (auto h = hierarchy.rbegin(); h != hierarchy.rend(); h++) {
                Type *t = *h;
                auto it = target.find(t->GetName());
                if (it == target.end()) {
                    modified = true;
                    for (auto &property : t->GetBlueprint()) {
                        diffVector.push_back({false, property[2]});
                    }
                } else {
                    auto &indexMap = it->second;
                    for (auto &property : t->GetBlueprint()) {
                        auto it = indexMap.find({property[0].get<string>(), property[1].get<string>()});
                        if (it == indexMap.end()) {
                            modified = true;
                            diffVector.push_back({false, property[2]});
                        } else {
                            diffVector.push_back({true, json(it->second)});
                            if (it->second != idx++) {
                                modified = true;
                            }
                        }
                    }
                }
            }
            if (!modified) {
                diff.erase(type->GetName());
            }
        }

        auto applyDiff = [](const vector<pair<bool, json>> &entityDiff, json &before) {
            json after;
            for (auto &p : entityDiff) {
                if (p.first) {
                    after.push_back(before[p.second.get<int>()]);
                } else {
                    after.push_back(p.second);
                }
            }
            before = after;
        };

        if (!diff.empty()) {
            // modify class blueprints
            WriteBlueprints(js["blueprints"]);

            // modify project setting
            {
                auto it = diff.find(ProjectSetting::StaticType()->GetName());
                if (it != diff.end()) {
                    applyDiff(it->second, js["setting"]);
                }
            }

            // modify assets
            json &assets = js["assets"];
            for (json::iterator i = assets.begin(); i != assets.end(); i++) {
                Type *type = Type::GetType(i.key());
                auto it = diff.find(type->GetName());
                if (it == diff.end()) {
                    continue;
                }
                for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                    applyDiff(it->second, j.value());
                }
            }
            
            // modify project file
            ofstream fs(fspath);
            if (fs.fail()) {
                throw exception();
            }
            fs << js;

            // modify scenes
            json &scenes = js["scenes"];
            for (json &scene : scenes) {
                json js;
                auto path = filesystem::u8path(directory + "/" + scene.get<string>());
                ifstream ifs(path);
                if (ifs.fail()) {
                    continue;
                }
                ifs >> js;
                json &entities = js["entities"];
                for (json::iterator i = entities.begin(); i != entities.end(); i++) {
                    Type *type = Type::GetType(i.key());
                    auto it = diff.find(type->GetName());
                    if (it == diff.end()) {
                        continue;
                    }
                    for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                        applyDiff(it->second, j.value());
                    }
                }

                ofstream ofs(path);
                if (ofs.fail()) {
                    continue;
                }
                ofs << js;
            }

            // modify prefabs
            auto it = assets.find(APrefab::StaticType()->GetName());
            if (it != assets.end()) {
                json &prefabs = *it;
                for (json::iterator i = prefabs.begin(); i != prefabs.end(); i++) {
                    json js;
                    auto path = filesystem::u8path(directory + "/" + i.value()[2].get<string>()); // path index is fixed as 2
                    ifstream ifs(path);
                    if (ifs.fail()) {
                        continue;
                    }
                    ifs >> js;
                    json &entities = js["entities"];
                    for (json::iterator i = entities.begin(); i != entities.end(); i++) {
                        Type *type = Type::GetType(i.key());
                        auto it = diff.find(type->GetName());
                        if (it == diff.end()) {
                            continue;
                        }
                        for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                            applyDiff(it->second, j.value());
                        }
                    }

                    ofstream ofs(path);
                    if (ofs.fail()) {
                        continue;
                    }
                    ofs << js;
                }
            }
        }

        // read scenes
        scenes = js["scenes"].get<unordered_set<string>>();
        
        // read assets
        json &assets = js["assets"];
        for (json::iterator i = assets.begin(); i != assets.end(); i++) {
            Type *type = Type::GetType(i.key());
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                Entity *entity = type->Instantiate();
                this->assets.insert({stoull(j.key()), (Asset *)entity});
            }
        }
        
        for (json::iterator i = assets.begin(); i != assets.end(); i++) {
            Type *type = Type::GetType(i.key());
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                type->Deserialize(j.value(), this->assets.at(stoull(j.key())));
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
        
        // write class blueprints
        WriteBlueprints(js["blueprints"]);

        // write project setting
        ProjectSetting::StaticType()->Serialize(js["setting"], setting);

        // write scenes
        js["scenes"] = scenes;

        // write assets
        json &assets = js["assets"];
        for (auto &p : this->assets) {
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

void Project::WriteBlueprints(json &blueprints) {
    blueprints.clear();
    for (auto &p : Type::GetAllTypes()) {
        Type *type = p.second;
        json &blueprint = blueprints[type->GetName()];
        Type *temp = type;
        int base = 0;
        while (temp = temp->GetBase()) {
            base += (int)temp->GetBlueprint().size();
        }
        blueprint.push_back(base);
        blueprint.push_back(vector<vector<string>>());
        for (auto &property : type->GetBlueprint()) {
            blueprint[1].push_back(vector<string>{property[0].get<string>(), property[1].get<string>()});
        }
    }
}