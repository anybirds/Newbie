#include <tsl/htrie_map.h>

#include <Engine.hpp>
#include <InspectorPanel.hpp>
#include <AssetPanel.hpp>

using namespace std;
using namespace tsl;
using namespace glm;

InspectorPanel::InspectorPanel() : Panel("Inspector") {
    GetVisualizeProperty() = [](const string &name, const function<void(void *)> &visualize, void *pointer) {
        if (ImGui::BeginTable(name.c_str(), 2, ImGuiTableFlags_None)) {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, ImGui::GetContentRegionAvail().x * 0.2f);
            ImGui::TableSetupColumn("");
            
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text(name.c_str());
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            visualize(pointer);
            ImGui::PopItemWidth();
            ImGui::EndTable();
        }
    };
    GetVisualize<bool, true>() = [](void *p) {
        ImGui::Checkbox("", (bool *)p);
    };
    GetVisualize<bool, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<bool, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<uint8_t, true>() = [](void *p) {
        ImGui::InputScalar("", ImGuiDataType_U8, (uint8_t *)p);
    };
    GetVisualize<uint8_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<uint8_t, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<int8_t, true>() = [](void *p) {
        ImGui::InputScalar("", ImGuiDataType_S8, (int8_t *)p);
    };
    GetVisualize<int8_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<int8_t, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<uint16_t, true>() = [](void *p) {
        ImGui::InputScalar("", ImGuiDataType_U16, (uint16_t *)p);
    };
    GetVisualize<uint16_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<uint16_t, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<int16_t, true>() = [](void *p) {
        ImGui::InputScalar("", ImGuiDataType_S16, (int16_t *)p);
    };
    GetVisualize<int16_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<int16_t, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<uint32_t, true>() = [](void *p) {
        ImGui::InputScalar("", ImGuiDataType_U32, (uint32_t *)p);
    };
    GetVisualize<uint32_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<uint32_t, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<int32_t, true>() = [](void *p) {
        ImGui::InputScalar("", ImGuiDataType_S32, (int32_t *)p);
    };
    GetVisualize<int32_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<int32_t, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<uint64_t, true>() = [](void *p) {
        ImGui::InputScalar("", ImGuiDataType_U64, (uint64_t *)p);
    };
    GetVisualize<uint64_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<uint64_t, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<int64_t, true>() = [](void *p) {
        ImGui::InputScalar("", ImGuiDataType_S64, (int64_t *)p);
    };
    GetVisualize<int64_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<int64_t, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<float, true>() = [](void *p) {
        ImGui::InputScalar("", ImGuiDataType_Float, (float *)p);
    };
    GetVisualize<float, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<float, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<double, true>() = [](void *p) {
        ImGui::InputScalar("", ImGuiDataType_Double, (double *)p);
    };
    GetVisualize<double, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<double, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<string, true>() = [](void *p) {
        ImGui::InputText("", (string *)p);
    };
    GetVisualize<string, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<string, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<vec2, true>() = [](void *p) {
        ImGui::InputFloat2("", (float *)p);
    };
    GetVisualize<vec2, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<vec2, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<vec3, true>() = [](void *p) {
        ImGui::InputFloat3("", (float *)p);
    };
    GetVisualize<vec3, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<vec3, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<vec4, true>() = [](void *p) {
        ImGui::InputFloat4("", (float *)p);
    };
    GetVisualize<vec4, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<vec4, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<mat2, true>() = [](void *p) {
        mat2 &v = *(mat2 *)p;
        mat2 t = transpose(v);
        ImGui::InputFloat2("##0", (float *)&t[0][0]);
        ImGui::InputFloat2("##1", (float *)&t[1][0]);
        v = transpose(t);
    };
    GetVisualize<mat2, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<mat2, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<mat3, true>() = [](void *p) {
        mat3 &v = *(mat3 *)p;
        mat3 t = transpose(v);
        ImGui::InputFloat3("##0", (float *)&t[0][0]);
        ImGui::InputFloat3("##1", (float *)&t[1][0]);
        ImGui::InputFloat3("##2", (float *)&t[2][0]);
        v = transpose(t);
    };
    GetVisualize<mat3, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        mat3 &v = *(mat3 *)p;
        GetVisualize<mat3, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<mat4, true>() = [](void *p) {
        mat4 &v = *(mat4 *)p;
        mat4 t = transpose(v);
        ImGui::InputFloat4("##0", (float *)&t[0][0]);
        ImGui::InputFloat4("##1", (float *)&t[1][0]);
        ImGui::InputFloat4("##2", (float *)&t[2][0]);
        ImGui::InputFloat4("##3", (float *)&t[3][0]);
        v = transpose(t);
    };
    GetVisualize<mat4, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<mat4, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<quat, true>() = [](void *p) {
        ImGui::InputFloat4("", (float *)p);
    };
    GetVisualize<quat, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<quat, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<Entity *, true>() = [this](void *p) {
        pair<Type *, void *> &v = *(pair<Type *, void *> *)p;
        Type *type = v.first;
        Entity *&entity = *(Entity **)v.second;
        const char *icon = GetIconCharacter(type);
        string name;
        if (type == GameObject::StaticType()) {
            GameObject *gameObject = (GameObject *)entity;
            if (gameObject && !gameObject->IsRemoved()) {
                name = icon + gameObject->GetName();
            }
            if (ImGui::BeginCombo("", name.c_str())) {
                function<void(GameObject *item)> show = [&](GameObject *item) {
                    for (Transform *t : item->GetTransform()->GetChildren()) {
                        show(t->GetGameObject());
                    }
                    ImGui::PushID(item);
                    if (ImGui::Selectable((icon + item->GetName()).c_str(), item == gameObject)) {
                        entity = (Entity *)item;
                    }
                    ImGui::PopID();
                    if (item == gameObject) { ImGui::SetItemDefaultFocus(); }
                };
                show(Scene::GetInstance().GetRoot());
                if (ImGui::Selectable("")) {
                    entity = nullptr;
                }
                ImGui::EndCombo();
            }
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
                    IM_ASSERT(payload->DataSize == sizeof(GameObject *));
                    GameObject *gameObject = *(GameObject **)payload->Data;
                    entity = (Entity *)gameObject;
                }
                ImGui::EndDragDropTarget();
            }
        } else if (Type::IsBaseOf(Component::StaticType(), type)) {
            Component *component = (Component *)entity;
            if (component && !component->IsRemoved()) {
                name = icon + component->GetGameObject()->GetName();
            }
            if (ImGui::BeginCombo("", name.c_str())) {
                vector<Component *> items = Scene::GetInstance().GetRoot()->FindComponents(type);
                for (Component *item : items) {
                    ImGui::PushID(item);
                    if (ImGui::Selectable((GetIconCharacter(item->GetType()) + item->GetGameObject()->GetName()).c_str(), item == component)) {
                        entity = (Entity *)item;
                    }
                    ImGui::PopID();
                    if (item == component) { ImGui::SetItemDefaultFocus(); }
                }
                if (ImGui::Selectable("")) {
                    entity = nullptr;
                }
                ImGui::EndCombo();
            }
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
                    IM_ASSERT(payload->DataSize == sizeof(GameObject *));
                    GameObject *gameObject = *(GameObject **)payload->Data;
                    if (Component *component = gameObject->GetComponent(type)) {
                        entity = (Entity *)component;
                    }
                }
                ImGui::EndDragDropTarget();
            }
        } else if (Type::IsBaseOf(Asset::StaticType(), type)) {
            Asset *asset = (Asset *)entity;
            if (asset && !asset->IsRemoved()) {
                name = icon + asset->GetName();
            }
            if (ImGui::BeginCombo("", name.c_str())) {
                Project &project = Project::GetInstance();
                vector<Asset *> items = project.GetAssets(type);
                for (Asset *item : items) {
                    ImGui::PushID(item);
                    if (ImGui::Selectable((GetIconCharacter(item->GetType()) + item->GetName()).c_str(), item == asset)) {
                        entity = (Entity *)item;
                    }
                    ImGui::PopID();
                    if (item == asset) { ImGui::SetItemDefaultFocus(); }
                }
                if (ImGui::Selectable("")) {
                    entity = nullptr;
                }
                ImGui::EndCombo();
            }
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset")) {
                    IM_ASSERT(payload->DataSize == sizeof(Asset *));
                    Asset *asset = *(Asset **)payload->Data;
                    if (Type::IsBaseOf(type, asset->GetType())) {
                        entity = (Entity *)asset;
                    }
                }
                ImGui::EndDragDropTarget();
            }
        } else {
            if (entity) {
                if (Type::IsBaseOf(GameObject::StaticType(), entity->GetType())) {
                    name = icon + ((GameObject *)entity)->GetName();
                } else if (Type::IsBaseOf(Component::StaticType(), entity->GetType())) {
                    name = icon + ((Component *)entity)->GetGameObject()->GetName();
                } else if (Type::IsBaseOf(Asset::StaticType(), entity->GetType())) {
                    name = icon + ((Asset *)entity)->GetName();
                } else {
                    name = icon;
                }
            }
            if (ImGui::BeginCombo("", name.c_str())) {
                if (ImGui::Selectable("")) {
                    entity = nullptr;
                }
                ImGui::EndCombo();
            }
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
                    IM_ASSERT(payload->DataSize == sizeof(GameObject *));
                    GameObject *gameObject = *(GameObject **)payload->Data;
                    entity = (Entity *)gameObject;
                }
                ImGui::EndDragDropTarget();
            }
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
                    IM_ASSERT(payload->DataSize == sizeof(GameObject *));
                    GameObject *gameObject = *(GameObject **)payload->Data;
                    if (Component *component = gameObject->GetComponent(type)) {
                        entity = (Entity *)component;
                    }
                }
                ImGui::EndDragDropTarget();
            }
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset")) {
                    IM_ASSERT(payload->DataSize == sizeof(Asset *));
                    Asset *asset = *(Asset **)payload->Data;
                    if (Type::IsBaseOf(type, asset->GetType())) {
                        entity = (Entity *)asset;
                    }
                }
                ImGui::EndDragDropTarget();
            }
        }
    };
    GetVisualize<Entity *, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<Entity *, true>()(p);
        ImGui::PopDisabled();
    };
    GetVisualize<shared_ptr<Resource>, true>() = [this](void *p) {
        pair<Type *, void *> &v = *(pair<Type *, void *> *)p;
        Type *type = v.first;
        shared_ptr<Resource> &resource = *(shared_ptr<Resource> *)v.second;
        const char *icon = GetIconCharacter(type);
        string name;
        if (resource) {
            name = icon + resource->GetName();
        }
        if (ImGui::BeginCombo("", name.c_str())) {
            Project &project = Project::GetInstance();
            vector<Asset *> items = project.GetAssets(type);
            for (Asset *item : items) {
                ImGui::PushID(item);
                if (ImGui::Selectable((GetIconCharacter(item->GetType()) + item->GetName()).c_str(), resource && item == resource->GetAsset())) {
                    resource = item->GetResource();
                }
                ImGui::PopID();
                if (resource && item == resource->GetAsset()) { ImGui::SetItemDefaultFocus(); }
            }
            if (ImGui::Selectable("")) {
                resource.reset();
            }
            ImGui::EndCombo();
        }
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset")) {
                IM_ASSERT(payload->DataSize == sizeof(Asset *));
                Asset *asset = *(Asset **)payload->Data;
                if (Type::IsBaseOf(type, asset->GetType())) {
                    resource = asset->GetResource();
                }
            }
            ImGui::EndDragDropTarget();
        }
    };
    GetVisualize<shared_ptr<Resource>, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<shared_ptr<Resource>, true>()(p);
        ImGui::PopDisabled();
    };
    
    GetVisualize<pair, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        pair<void *, void *> &v = *(pair<void *, void *> *)p;
        if (ImGui::BeginTable("##", 2, ImGuiTableFlags_None)) {
            ImGui::TableSetupColumn("");
            ImGui::TableSetupColumn("");
            
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::PushID(0);
            f[0](v.first);
            ImGui::PopID();
            ImGui::PopItemWidth();

            ImGui::TableNextColumn();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::PushID(1);
            f[1](v.second);
            ImGui::PopID();
            ImGui::PopItemWidth();

            ImGui::EndTable();
        }
    };
    GetVisualize<pair, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<pair, true>()(f, p);
        ImGui::PopDisabled();
    };
    GetVisualize<vector, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        vector<void *> &v = *(vector<void *> *)p;
        int j = -1;
        if (ImGui::TreeNodeEx("##", ImGuiTreeNodeFlags_OpenOnArrow)) {
            int size = (int)v.size();
            for (int i = 0; i < size; i++) {
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ChildBg]);
                if (ImGui::Button(ICON_FA_TIMES)) {
                    j = i;
                }
                ImGui::PopStyleColor();
                ImGui::SameLine();
                f[0](v[i]);
                ImGui::PopID();
            }
            if (ImGui::Button(ICON_FA_PLUS)) {
                f[1](nullptr);
            }
            if (j >= 0) {
                f[2]((void *)(uintptr_t)j);
            }
            ImGui::TreePop();
        }
    };
    GetVisualize<vector, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::PushDisabled(true);    
        GetVisualize<vector, true>()(f, p);
        ImGui::PopDisabled();
    };
    GetVisualize<list, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        vector<void *> &v = *(vector<void *> *)p;
        int j = -1;
        if (ImGui::TreeNodeEx("##", ImGuiTreeNodeFlags_OpenOnArrow)) {
            int size = (int)v.size();
            for (int i = 0; i < size; i++) {
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ChildBg]);
                if (ImGui::Button(ICON_FA_TIMES)) {
                    j = i;
                }
                ImGui::PopStyleColor();
                ImGui::SameLine();
                f[0](v[i]);
                ImGui::PopID();
            }
            if (ImGui::Button(ICON_FA_PLUS)) {
                f[1](nullptr);
            }
            if (j >= 0) {
                f[2]((void *)(uintptr_t)j);
            }
            ImGui::TreePop();
        }
    };
    GetVisualize<list, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<list, true>()(f, p);
        ImGui::PopDisabled();
    };
    GetVisualize<set, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        vector<void *> &v = *(vector<void *> *)p;
        void *j = nullptr;
        if (ImGui::TreeNodeEx("##", ImGuiTreeNodeFlags_OpenOnArrow)) {
            int size = (int)v.size();
            for (int i = 0; i < size; i++) {
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ChildBg]);
                if (ImGui::Button(ICON_FA_TIMES)) {
                    j = v[i];
                }
                ImGui::PopStyleColor();
                ImGui::SameLine();
                f[0](v[i]);
                ImGui::PopID();
            }

            ImGuiStorage *storage = ImGui::GetStateStorage();
            ImGuiID id = ImGui::GetID(ICON_FA_PLUS);
            void *in = storage->GetVoidPtr(id);
            if (ImGui::Button(ICON_FA_PLUS)) {
                if (in) {
                    f[4](in); // insert
                    f[3](in); // delete
                    in = nullptr;
                    storage->SetVoidPtr(id, in);
                } else {
                    f[2]((void *)&in); // new
                    storage->SetVoidPtr(id, in);
                }
            }
            if (in) {
                ImGui::SameLine();
                f[1](in); // visualize
            }
            if (j) {
                f[5](j); // erase
            }
            ImGui::TreePop();
        }
    };
    GetVisualize<set, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<set, true>()(f, p);
        ImGui::PopDisabled();
    };
    GetVisualize<unordered_set, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        vector<void *> &v = *(vector<void *> *)p;
        void *j = nullptr;
        if (ImGui::TreeNodeEx("##", ImGuiTreeNodeFlags_OpenOnArrow)) {
            int size = (int)v.size();
            for (int i = 0; i < size; i++) {
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ChildBg]);
                if (ImGui::Button(ICON_FA_TIMES)) {
                    j = v[i];
                }
                ImGui::PopStyleColor();
                ImGui::SameLine();
                f[0](v[i]);
                ImGui::PopID();
            }

            ImGuiStorage *storage = ImGui::GetStateStorage();
            ImGuiID id = ImGui::GetID(ICON_FA_PLUS);
            void *in = storage->GetVoidPtr(id);
            if (ImGui::Button(ICON_FA_PLUS)) {
                if (in) {
                    f[4](in); // insert
                    f[3](in); // delete
                    in = nullptr;
                    storage->SetVoidPtr(id, in);
                } else {
                    f[2]((void *)&in); // new
                    storage->SetVoidPtr(id, in);
                }
            }
            if (in) {
                ImGui::SameLine();
                f[1](in); // visualize
            }
            if (j) {
                f[5](j); // erase
            }
            ImGui::TreePop();
        }
    };
    GetVisualize<unordered_set, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<unordered_set, true>()(f, p);
        ImGui::PopDisabled();
    };
    GetVisualize<map, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        vector<void *> &v = *(vector<void *> *)p;
        void *j = nullptr;
        if (ImGui::TreeNodeEx("##", ImGuiTreeNodeFlags_OpenOnArrow)) {
            int size = (int)v.size();
            for (int i = 0; i < size; i++) {
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ChildBg]);
                if (ImGui::Button(ICON_FA_TIMES)) {
                    j = v[i];
                }
                ImGui::PopStyleColor();
                ImGui::SameLine();
                f[0](v[i]);
                ImGui::PopID();
            }

            ImGuiStorage *storage = ImGui::GetStateStorage();
            ImGuiID id = ImGui::GetID(ICON_FA_PLUS);
            void *in = storage->GetVoidPtr(id);
            if (ImGui::Button(ICON_FA_PLUS)) {
                if (in) {
                    f[4](in); // insert
                    f[3](in); // delete
                    in = nullptr;
                    storage->SetVoidPtr(id, in);
                } else {
                    f[2]((void *)&in); // new
                    storage->SetVoidPtr(id, in);
                }
            }
            if (in) {
                ImGui::SameLine();
                f[1](in); // visualize
            }
            if (j) {
                f[5](j); // erase
            }
            ImGui::TreePop();
        }
    };
    GetVisualize<map, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<map, true>()(f, p);
        ImGui::PopDisabled();
    };
    GetVisualize<unordered_map, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        vector<void *> &v = *(vector<void *> *)p;
        void *j = nullptr;
        if (ImGui::TreeNodeEx("##", ImGuiTreeNodeFlags_OpenOnArrow)) {
            int size = (int)v.size();
            for (int i = 0; i < size; i++) {
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ChildBg]);
                if (ImGui::Button(ICON_FA_TIMES)) {
                    j = v[i];
                }
                ImGui::PopStyleColor();
                ImGui::SameLine();
                f[0](v[i]);
                ImGui::PopID();
            }

            ImGuiStorage *storage = ImGui::GetStateStorage();
            ImGuiID id = ImGui::GetID(ICON_FA_PLUS);
            void *in = storage->GetVoidPtr(id);
            if (ImGui::Button(ICON_FA_PLUS)) {
                if (in) {
                    f[4](in); // insert
                    f[3](in); // delete
                    in = nullptr;
                    storage->SetVoidPtr(id, in);
                } else {
                    f[2]((void *)&in); // new
                    storage->SetVoidPtr(id, in);
                }
            }
            if (in) {
                ImGui::SameLine();
                f[1](in); // visualize
            }
            if (j) {
                f[5](j); // erase
            }
            ImGui::TreePop();
        }
    };
    GetVisualize<unordered_map, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::PushDisabled(true);
        GetVisualize<unordered_map, true>()(f, p);
        ImGui::PopDisabled();
    };
}

void InspectorPanel::ShowContents() {
    Project &project = Project::GetInstance();
    if (!project.IsLoaded()) {
        ImGui::End();
        return;
    }

    Component *hovered = nullptr;
    Entity *entity = GetSelected();
    if (entity) {
        if (entity->GetType() == GameObject::StaticType()) {
            GameObject *gameObject = (GameObject *)entity;
            ShowIcon(GameObject::StaticType());
            ImGui::SameLine();
            ImGui::Text(gameObject->GetName().c_str());
            for (Component *component : gameObject->GetAllComponents()) {
                ImGui::Separator();
                Type *type = component->GetType();
                ShowIcon(type);
                ImGui::SameLine();
                ImGui::PushID(component);
                ImGui::Selectable(type->GetName().c_str());
                if (ImGui::IsItemHovered()) {
                    hovered = component;
                }
                type->Visualize(component);
                ImGui::PopID();
            }

            if (ImGui::BeginPopupContextWindow())
            {
                static Component *component;
                if (!menu) {
                    component = hovered;
                }
                if (ImGui::MenuItem("Cut", nullptr, nullptr, component && component->GetType() != Transform::StaticType())) {
                    copyedType = component->GetType();
                    copyedType->Serialize(copyed, component);
                    component->Remove();
                }
                if (ImGui::MenuItem("Copy", nullptr, nullptr, component && component->GetType() != Transform::StaticType())) {
                    copyedType = component->GetType();
                    copyedType->Serialize(copyed, component);
                }
                if (ImGui::MenuItem("Paste", nullptr, nullptr, !copyed.empty())) {
                    gameObject->AddComponent(copyedType, copyed);
                }
                ImGui::Separator();
                if (ImGui::BeginMenu("Add Component")) {
                    // prefix-search asset types
                    ImGui::Text(ICON_FA_SEARCH);
                    ImGui::SameLine();
                    ImGui::PushItemWidth(-FLT_MIN);
                    ImGui::InputText("##Search", &searchAdd);
                    ImGui::PopItemWidth();
                    ImGui::Separator();

                    htrie_map<char, Type *> trie;
                    for (auto &p : Type::GetAllTypes()) {
                        Type *type = p.second;
                        if (!type->IsAbstract() && type != Transform::StaticType() && Type::IsBaseOf(Component::StaticType(), type)) {
                            trie.insert(type->GetName(), type);   
                        }
                    }
                    auto range = trie.equal_prefix_range(searchAdd);
                    
                    for (auto it = range.first; it != range.second; it++) {
                        Type *type = *it;
                        if (ImGui::MenuItem((GetIconCharacter(type) + type->GetName()).c_str())) {
                            gameObject->AddComponent(type);
                        }
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Remove", nullptr, nullptr, component && component->GetType() != Transform::StaticType())) {
                    component->Remove();
                }
                
                menu = true;
                ImGui::EndPopup();
            } else {
                menu = false;
            }
        } else if (Type::IsBaseOf(Asset::StaticType(), entity->GetType())) {
            Asset *asset = (Asset *)entity;
            Type *type = asset->GetType();
            ShowIcon(type);
            ImGui::SameLine();
            ImGui::Text(asset->GetName().c_str());
            ImGui::Separator();
            ImGui::PushID(asset);
            type->Visualize(entity);
            ImGui::PopID();
            
            // show Resource
            if (asset->IsResourced()) {
                ImGui::Separator();
                ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 95.0f);
                if (ImGui::Button("Apply", ImVec2(80.0f, 0.0f))) {
                    asset->Apply();
                }
                shared_ptr<Resource> resource = asset->GetResource();
                type = resource->GetType();
                if (ImGui::CollapsingHeader(type->GetName().c_str())) {
                    type->Visualize(resource.get());
                }
            }
        }
    }
}