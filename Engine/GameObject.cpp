#include <cstdint>

#include <GameObject.hpp>
#include <Component.hpp>
#include <Prefab.hpp>

using namespace std;
using namespace Engine;
using json = nlohmann::json;

void GameObject::ToJson(json &js, const vector<GameObject *> &roots, bool nullify, const shared_ptr<Prefab> &base, uintptr_t start) {
    GetMap().clear();
    SetNullify(nullify);
    js.clear();

    if (nullify) {
        if (base) {
            uintptr_t i = 1;
            uintptr_t j = start;
            function<void(GameObject *, GameObject *)> recurse = [&recurse, &i, &j](GameObject *gameObject, GameObject *base) {
                auto it = gameObject->GetTransform()->children.begin();
                if (base) {
                    for (Transform *t : base->GetTransform()->children) {
                        recurse((*it)->GetGameObject(), t->GetGameObject());
                        it++;
                    }
                }
                while (it != gameObject->GetTransform()->children.end()) {
                    recurse((*it)->GetGameObject(), nullptr);
                    it++;
                }

                auto jt = gameObject->components.begin();
                if (base) {
                    for (Component *component : base->components) {
                        GetMap().insert(make_pair((uintptr_t)*jt, i++));
                        jt++;
                    }
                }
                while (jt != gameObject->components.end()) {
                    GetMap().insert(make_pair((uintptr_t)*jt, j++));
                    jt++;
                }

                if (base) {
                    GetMap().insert(make_pair((uintptr_t)gameObject, i++));
                } else {
                    GetMap().insert(make_pair((uintptr_t)gameObject, j++));
                }
            };
            for (GameObject *root : roots) {
                recurse(root, base->GetGameObject());
            }
        } else {
            uintptr_t id = 1;
            function<void(GameObject *)> recurse = [&recurse, &id](GameObject *gameObject) {
                Transform *transform = gameObject->GetTransform();
                for (Transform *t : transform->children) {
                    recurse(t->GetGameObject());
                }

                for (Component *component : gameObject->components) {
                    GetMap().insert(make_pair((uintptr_t)component, id++));
                }
                GetMap().insert(make_pair((uintptr_t)gameObject, id++));
            };
            for (GameObject *root : roots) {
                recurse(root);
            }
        }
    }

    js["roots"] = roots;

    // write entities
    json& entities = js["entities"];
    function<void(GameObject *)> recurse = [&recurse, &entities](GameObject *gameObject) {
        Transform *transform = gameObject->GetTransform();
        for (Transform *t : transform->children) {
            recurse(t->GetGameObject());
        }

        for (Component *component : gameObject->components) {
            uintptr_t id = GetNullify() ? GetMap()[(uintptr_t)component] : (uintptr_t)component;
            Type *type = component->GetType();
                type->Serialize(
                    entities[type->GetName()][to_string(id)],
                    component); 
        }

        uintptr_t id = GetNullify() ? GetMap()[(uintptr_t)gameObject] : (uintptr_t)gameObject;
        GameObject::StaticType()->Serialize(
            entities[GameObject::StaticType()->GetName()][to_string(id)], 
            gameObject);
    };
    for (GameObject *root : roots) {
        recurse(root);
    }
}

uintptr_t GameObject::FromJson(const json &js, vector<GameObject *> &roots, bool nullify) {
    GetMap().clear();
    SetNullify(nullify);

    // read entities
    const json &entities = js["entities"];
    for (json::const_iterator i = entities.begin(); i != entities.end(); i++) {
        const Type *type = Type::GetType(i.key());
        for (json::const_iterator j = i.value().begin(); j != i.value().end(); j++) {
            Entity *entity = type->Instantiate();
            GetMap().insert({stoull(j.key()), (uintptr_t)entity});
        }
    }

    for (json::const_iterator i = entities.begin(); i != entities.end(); i++) {
        const Type *type = Type::GetType(i.key());
        for (json::const_iterator j = i.value().begin(); j != i.value().end(); j++) {
            type->Deserialize(j.value(), (Entity *)GetMap().at(stoull(j.key())));
        }
    }
    
    // read roots 
    roots = js["roots"].get<vector<GameObject *>>();
    
    uintptr_t entityCount = GetMap().size();
    GetMap().clear();

    return entityCount;
}

GameObject *GameObject::GetCopy() {
    json js;
    vector<GameObject *> roots;
    ToJson(js, vector<GameObject *>{this}, false);
    FromJson(js, roots, false);
    return *roots.begin();
}

GameObject *GameObject::AddGameObject() {
    GameObject *child = new GameObject();
    Transform *t = child->AddComponent<Transform>();
    t->parent = transform;
    transform->children.push_back(t);
    return child;
}

GameObject *GameObject::AddGameObject(GameObject *gameObject) {
    GameObject *child = gameObject->GetCopy();
    Transform *t = child->GetTransform();
    t->parent = transform;
    t->Propagate();
    transform->children.push_back(t);
    return child;
}

GameObject *GameObject::AddGameObject(const std::shared_ptr<Prefab> &prefab) {
    vector<GameObject *> roots;
    FromJson(prefab->GetJson(), roots);
    assert(roots.size() == 1);
    GameObject *child = roots[0];
    Transform *t = child->GetTransform();
    t->parent = transform;
    t->Propagate();
    transform->children.push_back(t);
    return child;
}

GameObject *GameObject::FindGameObject(const string &name) const {
    if (GetName() == name) {
        return const_cast<GameObject *>(this);
    }
    for (Transform *t : transform->GetChildren()) {
        GameObject *child = t->GetGameObject();
        if (child->FindGameObject(name)) {
            return child;
        }
    }
    return nullptr;
}

bool GameObject::HasPrefabCycle() const {
    function<bool(const GameObject *gameObject)> recurse = [this, &recurse](const GameObject *gameObject)->bool {
        bool cycle = false;
        for (Transform *t : gameObject->transform->GetChildren()) {
            cycle |= recurse(t->GetGameObject()) || t->GetGameObject()->GetPrefab().get() == prefab.get();
        }
        return cycle;
    };
    return recurse(this);
}