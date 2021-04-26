#include <cstdint>

#include <GameObject.hpp>
#include <Component.hpp>
#include <Asset.hpp>

using namespace std;
using namespace Engine;
using json = nlohmann::json;

void GameObject::ToJson(json &js, vector<GameObject *> &roots, bool nullify) {
    GetMap().clear();
    SetNullify(nullify);

    if (nullify) {
        uint64_t id = 1;
        function<void(GameObject *)> recurse = [&recurse, &id](GameObject *gameObject) {
            Transform *transform = gameObject->GetTransform();
            for (Transform *t : transform->children) {
                recurse(t->GetGameObject());
            }

            for (Component *component : gameObject->components) {
                GetMap().insert(make_pair((uint64_t)component, id++));
            }
            GetMap().insert(make_pair((uint64_t)gameObject, id++));
        };
        for (GameObject *root : roots) {
            recurse(root);
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
            uint64_t id = GetNullify() ? GetMap()[(uint64_t)component] : (uint64_t)component;
            Type *type = component->GetType();
                type->Serialize(
                    entities[type->GetName()][to_string(id)],
                    component); 
        }

        uint64_t id = GetNullify() ? GetMap()[(uint64_t)gameObject] : (uint64_t)gameObject;
        GameObject::StaticType()->Serialize(
            entities[GameObject::StaticType()->GetName()][to_string(id)], 
            gameObject);
    };
    for (GameObject *root : roots) {
        recurse(root);
    }
}

void GameObject::FromJson(json &js, vector<GameObject *> &roots, bool nullify) {
    GetMap().clear();
    SetNullify(nullify);

    // read entities
    json &entities = js["entities"];
    for (json::iterator i = entities.begin(); i != entities.end(); i++) {
        const Type *type = Type::GetType(i.key());
        for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
            Entity *entity = type->Instantiate();
            GetMap().insert({stoll(j.key()), (uint64_t)entity});
        }
    }

    for (json::iterator i = entities.begin(); i != entities.end(); i++) {
        const Type *type = Type::GetType(i.key());
        for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
            type->Deserialize(j.value(), (Entity *)GetMap().at(stoll(j.key())));
        }
    }
    
    // read roots 
    roots = js["roots"].get<vector<GameObject *>>();
    
    GetMap().clear();
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