#include <cstdint>
#include <functional>

#include <GameObject.hpp>
#include <Component.hpp>
#include <Prefab.hpp>

using namespace std;
using json = nlohmann::json;

void GameObject::ToJson(json &js, const GameObject *gameObject) {
    js.clear();

    // write root
    js["root"] = gameObject;

    // write entities
    json &entities = js["entities"];
    function<void(const GameObject *)> recurse = [&recurse, &entities](const GameObject *gameObject) {
        Transform *transform = gameObject->GetTransform();
        for (Transform *t : transform->children) {
            recurse(t->GetGameObject());
        }

        for (Component *component : gameObject->components) {
            Type *type = component->GetType();
                type->Serialize(
                    entities[type->GetName()][to_string((uintptr_t)component)],
                    component); 
        }

        GameObject::StaticType()->Serialize(
            entities[GameObject::StaticType()->GetName()][to_string((uintptr_t)gameObject)], 
            gameObject);
    };
    recurse(gameObject);
}

void GameObject::FromJson(const json &js, GameObject *&gameObject, bool nullify) {
    GetMap().clear();
    SetNullify(nullify);

    // read entities
    const json &entities = js["entities"];
    for (json::const_iterator i = entities.begin(); i != entities.end(); i++) {
        Type *type = Type::GetType(i.key());
        for (json::const_iterator j = i.value().begin(); j != i.value().end(); j++) {
            Entity *entity = type->Instantiate();
            GetMap().insert({stoull(j.key()), (uintptr_t)entity});
        }
    }

    for (json::const_iterator i = entities.begin(); i != entities.end(); i++) {
        Type *type = Type::GetType(i.key());
        for (json::const_iterator j = i.value().begin(); j != i.value().end(); j++) {
            type->Deserialize(j.value(), (Entity *)GetMap().at(stoull(j.key())));
        }
    }

    // read root
    gameObject = js["root"].get<GameObject *>();

    GetMap().clear();
}

GameObject *GameObject::AddGameObject() {
    GameObject *child = new GameObject();
    Transform *t = child->AddComponent<Transform>();
    t->parent = transform;
    transform->children.push_back(t);
    return child;
}

GameObject *GameObject::AddGameObject(GameObject *gameObject) {
    json js;
    GameObject *child;
    ToJson(js, gameObject);
    FromJson(js, child, false);
    Transform *t = child->GetTransform();
    t->parent = transform;
    transform->children.push_back(t);
    t->Propagate();
    return child;
}

GameObject *GameObject::AddGameObject(const std::shared_ptr<Prefab> &prefab) {
    GameObject *child;
    FromJson(prefab->GetJson(), child);
    child->SetParent(this);
    return child;
}

GameObject *GameObject::AddGameObject(const json &js) {
    GameObject *child;
    FromJson(js, child, false);
    Transform *t = child->GetTransform();
    t->parent = transform;
    transform->children.push_back(t);
    t->Propagate();
    return child;
}

GameObject *GameObject::FindGameObject(const string &name) const {
    if (GetName() == name) {
        return const_cast<GameObject *>(this);
    }
    for (Transform *t : transform->GetChildren()) {
        GameObject *child = t->GetGameObject();
        child->FindGameObject(name);
    }
    return nullptr;
}