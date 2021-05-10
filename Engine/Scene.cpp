#include <iostream>
#include <fstream>
#include <functional>
#include <filesystem>
#include <algorithm>

#include <Project.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Scene.hpp>
#include <Script.hpp>
#include <Prefab.hpp>
#include <Graphics/Drawer.hpp>
#include <Graphics/Renderer.hpp>

using json = nlohmann::json;
using namespace std;

void Scene::ToBackup() {
    Scene &scene = GetInstance();
    Scene &backup = GetBackup();
    backup.CloseImmediate();
    backup = scene;
    scene = Scene();
}

void Scene::FromBackup() {
    Scene &scene = GetInstance();
    Scene &backup = GetBackup();
    scene.CloseImmediate();
    scene = backup;
    backup = Scene();
}

void Scene::LoadBackup() {
    Scene &scene = GetInstance();
    Scene &backup = GetBackup();
    
    json js;
    backup.ToJson(js);
    scene.FromJson(js);

    scene.name = backup.name;
    scene.path = backup.path;
    scene.loaded = true;
}

void Scene::ToJson(json &js, bool nullify, const shared_ptr<Prefab> &base, uintptr_t start) {
    GameObject::ToJson(js, roots, nullify, base, start);
}

void Scene::FromJson(const json &js, bool nullify) {
    GameObject::FromJson(js, roots, nullify);
}

void Scene::Load(const string &path) {
    flags |= LOAD;
    this->loadPath = path;
}

void Scene::Save() {
    flags |= SAVE;
}

void Scene::Close() {
    flags |= CLOSE;
}

bool Scene::LoadImmediate(const string &path) {
    try {
        // open json file
        ifstream fs(filesystem::u8path(Project::GetInstance().GetDirectoy() + "/" + path));
        if (fs.fail()) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot open scene file: " << path << '\n';
            return false;
        }

        // move to backup in order to maintain duplicate resource and handle failure cases
        ToBackup();

        // get scene file path
        this->path = path;
        name = filesystem::u8path(this->path).stem().string();

        // read json object
        json js;
        fs >> js;
        FromJson(js);
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot read scene: " << path << '\n';
        FromBackup();
        return false;
    }
    
    GetBackup().CloseImmediate();

    loaded = true;
    cerr << '[' << __FUNCTION__ << ']' << " read scene: " << path << " done.\n";
    return true;
}

bool Scene::SaveImmediate() {
    try {
        // write json object
        json js;
        ToJson(js);
        
        // open json file
        ofstream fs(filesystem::u8path(Project::GetInstance().GetDirectoy() + "/" + path));
        if (fs.fail()) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot open scene: " << path << '\n';
            return false;
        }
        fs << js;
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot save scene: " << path << '\n';
        return false;
    }

    cerr << '[' << __FUNCTION__ << ']' << " save scene: " << path << " done.\n";
    return true;
}

void Scene::CloseImmediate() {
    for (GameObject *root : roots) {
        DestroyGameObject(root);
    }

    for (auto &order : batches) {
        for (auto &batch : order.second) {
            delete batch.second;
        }
    }

    *this = Scene();
}

GameObject *Scene::AddGameObject() {
    GameObject *root = new GameObject();
    root->AddComponent<Transform>();
    roots.push_back(root);
    return root;
}

GameObject *Scene::AddGameObject(GameObject *gameObject) {
    GameObject *root = gameObject->GetCopy();
    Transform *t = root->GetTransform();
    t->parent = nullptr;
    t->Propagate();
    roots.push_back(root);
    return root;
}

GameObject *Scene::AddGameObject(const std::shared_ptr<Prefab> &prefab) {
    vector<GameObject *> roots;
    GameObject::FromJson(prefab->GetJson(), roots);
    assert(roots.size() == 1);
    GameObject *root = roots[0];
    Transform *t = root->GetTransform();
    t->parent = nullptr;
    t->Propagate();
    this->roots.push_back(root);
    return root;
}

void Scene::RemoveGameObject(GameObject *gameObject) {
    gameObject->GetTransform()->state |= Component::REMOVED;
    gameObject->GetTransform()->SetLocalEnabled(false);
}

void Scene::RemoveComponent(Component *component) {
    component->state |= Component::REMOVED;
    component->SetLocalEnabled(false);
}

GameObject *Scene::FindGameObject(const string &name) {
    for (GameObject *root : roots) {
        if (GameObject *gameObject = root->FindGameObject(name)) {
            return gameObject;
        }
    }
    return nullptr;
}

void Scene::DestroyGameObject(GameObject *gameObject) {
    Transform *transform = gameObject->GetTransform();
    for (Transform *t : transform->GetChildren()) {
        DestroyGameObject(t->GetGameObject());
    }

    auto components = gameObject->components;
    for (Component *component : components) {
        delete component;
    }
}

void Scene::OrganizeGameObject(GameObject *gameObject) {
    vector<Component *> removeComps;
    vector<Transform *> removeTrans;
    function<void(GameObject *, bool)> remove = [&remove, &removeComps, &removeTrans](GameObject *gameObject, bool removed) {
        Transform *transform = gameObject->GetTransform();
        for (Transform *t : transform->GetChildren()) {
            remove(t->GetGameObject(), removed || t->IsRemoved());
        }
        if (removed || transform->IsRemoved()) {
            removeTrans.push_back(transform);
        }
        for (Component *component : gameObject->components) {
            if (removed || component->IsRemoved()) {
                removeComps.push_back(component);
            }
        }
    };
    remove(gameObject, gameObject->GetTransform()->IsRemoved());

    Remove(removeComps, removeTrans);
    Destroy(removeComps);
}

void Scene::Flags() {
    if (flags & CLOSE) {
        CloseImmediate();
    }
    if (flags & LOAD) {
        LoadImmediate(loadPath);
    }
    if (flags & SAVE) {
        SaveImmediate();
    }
    flags = 0U; 

    removeComps.clear();
    removeTrans.clear();
    enables.clear();
    disables.clear();

    set<APrefab *> dirtyAPrefabs;
    
    function<void(GameObject *, bool, bool)> recurse = [this, &recurse, &dirtyAPrefabs](GameObject *gameObject, bool enabled, bool removed) {
        shared_ptr<Prefab> prefab = gameObject->GetPrefab();
        if (prefab && prefab->GetDirty()) {
            APrefab *aprefab = (APrefab *)prefab->GetAsset();
            dirtyAPrefabs.insert(aprefab);

            json &baseEntities = prefab->GetJson()["entities"];
            json &assetEntities = aprefab->GetJson()["entities"];

            uintptr_t entityCount = 1U;
            for (auto ia = assetEntities.begin(); ia != assetEntities.end(); ia++) {
                entityCount += ia.value().size();
            }

            json js;
            GameObject::ToJson(js, vector<GameObject *>{gameObject}, true, prefab, entityCount);
            json &entities = js["entities"];

            for (json::iterator i = entities.begin(); i != entities.end(); i++) {
                auto ib = baseEntities.find(i.key());
                auto ia = assetEntities.find(i.key());
                if (ib != baseEntities.end()) {
                    assert(ia != assetEntities.end());
                    for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                        auto jb = ib.value().find(stoull(j.key()));
                        auto ja = ia.value().find(stoull(j.key()));
                        if (jb != ib.value().end()) {
                            assert(ja != ia.value().end());
                            for (size_t k = 0; k < j.value().size(); k++) {
                                if (j.value()[k] == jb.value()[k]) {
                                    // get value from asset
                                    j.value()[k] = ja.value()[k];
                                }
                            }   
                        }
                    }   
                }
            }

            for (json::iterator ia = assetEntities.begin(); ia != assetEntities.end(); ia++) {
                auto i = entities.find(ia.key());
                if (i == entities.end()) {
                    entities.insert(entities.end(), *ia);
                } else {
                    for (json::iterator ja = ia.value().begin(); ja != ia.value().end(); ja++) {
                        auto j = i.value().find(stoull(ja.key()));
                        if (j == i.value().end()) {
                            i.value().insert(i.value().end(), *ja);
                        }
                    }
                }
            } 
            json &gameObjects = entities["GameObject"];
            json &assetGameObjects = assetEntities["GameObject"];
            for (auto ia = assetGameObjects.begin(); ia != assetGameObjects.end(); ia++) {
                auto i = gameObjects.find(ia.key());
                json js = ia.value();
                size_t j = 0;
                while (j < ia.value()[0].size() && ia.value()[0][j] == i.value()[0][j]) {
                    j++;
                }
                for (; j < i.value()[0].size(); j++) {
                    js.push_back(i.value()[0][j]);
                }
                i.value() = js;
            }
            json &transforms = entities["Transform"];
            json &assetTransforms = assetEntities["Transform"];
            for (auto ia = assetTransforms.begin(); ia != assetTransforms.end(); ia++) {
                auto i = transforms.find(ia.key());
                json js = ia.value();
                size_t j = 0;
                while (j < ia.value()[2].size() && ia.value()[2][j] == i.value()[2][j]) {
                    j++;
                }
                for (; j < i.value()[2].size(); j++) {
                    js.push_back(i.value()[2][j]);
                }
                i.value() = js;
            }

            vector<GameObject *> roots;
            GameObject::FromJson(js, roots);

            Transform *parent = roots[0]->GetTransform()->GetParent();
            if (parent) {
                auto it = find(parent->children.begin(), parent->children.end(), gameObject);
                *it = roots[0]->GetTransform();
            } else {
                Scene &scene = Scene::GetInstance();
                auto it = find(scene.roots.begin(), scene.roots.end(), gameObject);
                *it = roots[0];
            }
            DestroyGameObject(gameObject);
        }
        
        Transform *transform = gameObject->GetTransform();
        for (Transform *t : transform->GetChildren()) {
            recurse(t->GetGameObject(), 
                enabled && t->IsLocalEnabled(), 
                removed || t->IsRemoved());
        }
        if (removed || transform->IsRemoved()) {
            removeTrans.push_back(transform);
        }

        for (Component *component : gameObject->components) {
            if (!(component->flags & Component::ENABLED) && (enabled && component->IsLocalEnabled())) {
                enables.push_back(component);
                component->flags |= Component::ENABLED;
            } else if ((component->flags & Component::ENABLED) && !(enabled && component->IsLocalEnabled())) {
                disables.push_back(component);
                component->flags &= ~Component::ENABLED;
            }
            if (removed || component->IsRemoved()) {
                removeComps.push_back(component);
            }
        }
    };

    for (GameObject *root : roots) {
        recurse(root, root->IsLocalEnabled(), root->IsRemoved());
    }

    for (APrefab *aprefab : dirtyAPrefabs) {
        vector<GameObject *> roots;
        GameObject::FromJson(aprefab->GetJson(), roots);
        OrganizeGameObject(roots[0]);
        GameObject::ToJson(aprefab->GetJson(), roots, true);
        aprefab->Apply();
        DestroyGameObject(roots[0]);
    }
}

void Scene::Track() {
    for (Component *component : enables) {
        component->OnTrack();
    }
}

void Scene::Untrack() {
    for (Component *component : disables) {
        component->OnUntrack();
    }
}

void Scene::Enable() {
    for (Component *component : enables) {
        try {
            component->OnEnable();
        } catch(...) {}
    }
}

void Scene::Disable() {
    for (Component *component : disables) {
        try {
            component->OnDisable();
        } catch(...) {}
    }
}

void Scene::Remove(const vector<Component *> &removeComps, const vector<Transform *> &removeTrans) {
    for (Component *component : removeComps) {
        try {
            component->OnDestroy();
        } catch(...) {}
    }
    sort(removeTrans.begin(), removeTrans.end(), [](const Transform *l, const Transform *r)->bool {
        return l->GetParent() < r->GetParent();
    });
    sort(removeComps.begin(), removeComps.end(), [](const Component *l, const Component *r)->bool {
        return l->GetGameObject() < r->GetGameObject();
    });
    for (size_t i = 0; i < removeTrans.size(); i++) {
        size_t j = i;
        do {
            removeTrans[j]->flags |= Component::DESTROYED;
            j++;
        } while (j < removeTrans.size() && removeTrans[j - 1]->GetParent() == removeTrans[j]->GetParent());
        Transform *transform = removeTrans[i]->GetParent();
        size_t l = 0;
        if (transform) {
            for (size_t k = 0; k < transform->children.size(); k++) {
                if (transform->children[k]->flags & Component::DESTROYED) {
                    continue;
                }
                transform->children[l] = transform->children[k];
                l++;
            }
            transform->children.resize(l);
        } else {
            for (size_t k = 0; k < roots.size(); k++) {
                if (roots[k]->GetTransform()->flags & Component::DESTROYED) {
                    continue;
                }
                roots[l] = roots[k];
                l++;
            }
            roots.resize(l);
        }
        i = j - 1;
    }
    for (size_t i = 0; i < removeComps.size(); i++) {
        size_t j = i;
        do {
            removeComps[j]->flags |= Component::DESTROYED;
            j++;
        } while (j < removeComps.size() && removeComps[j - 1]->GetGameObject() == removeComps[j]->GetGameObject());
        GameObject *gameObject = removeComps[i]->GetGameObject();
        size_t l = 0;
        for (size_t k = 0; k < gameObject->components.size(); k++) {
            if (gameObject->components[k]->flags & Component::DESTROYED) {
                continue;
            }
            gameObject->components[l] = gameObject->components[k];
            l++;
        }
        gameObject->components.resize(l);
        i = j - 1;
    }
}

void Scene::Destroy(const vector<Component *> &removeComps) {
    for (Component *component : removeComps) {
        delete component;
    }
}

void Scene::Update() {
    for (Script *script : scripts) {
        try {
            if (script->start) {
                script->Start();
                script->start = false;
            } else {
                script->Update();
            }
        } catch(...) {}
    }
}

void Scene::Render() {
    for (auto &order : renderers) {
        for (Renderer *renderer : order.second) {
            try {
                renderer->Render();
            } catch(...) {}
        }
    }
}

void Scene::Loop() {
    Flags();

    Untrack();
    Disable();
    Remove(removeComps, removeTrans);
    Destroy(removeComps);
    Track();
    Enable();
    Update();
    Render();
}

void Scene::PauseLoop() {
    Flags();

    Untrack();
    Remove(removeComps, removeTrans);
    Track();
    Render();
}

void Scene::RetainLoop() {
    Flags();

    Untrack();
    Track();
    Render();
}