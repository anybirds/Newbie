#include <Engine.hpp>
#include <InspectorPanel.hpp>
#include <AssetPanel.hpp>

using namespace std;
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
            visualize(pointer);
            ImGui::EndTable();
        }
    };
    GetVisualize<bool, true>() = [](void *p) {
        ImGui::Checkbox(("##" + to_string(uintptr_t(p))).c_str(), (bool *)p);
    };
    GetVisualize<bool, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::Checkbox(("##" + to_string(uintptr_t(p))).c_str(), (bool *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<uint8_t, true>() = [](void *p) {
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_U8, (uint8_t *)p);
    };
    GetVisualize<uint8_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_U8, (uint8_t *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<int8_t, true>() = [](void *p) {
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_S8, (int8_t *)p);
    };
    GetVisualize<int8_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_S8, (int8_t *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<uint16_t, true>() = [](void *p) {
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_U16, (uint16_t *)p);
    };
    GetVisualize<uint16_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_U16, (uint16_t *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<int16_t, true>() = [](void *p) {
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_S16, (int16_t *)p);
    };
    GetVisualize<int16_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_S16, (int16_t *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<uint32_t, true>() = [](void *p) {
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_U32, (uint32_t *)p);
    };
    GetVisualize<uint32_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_U32, (uint32_t *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<int32_t, true>() = [](void *p) {
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_S32, (int32_t *)p);
    };
    GetVisualize<int32_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_S32, (int32_t *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<uint64_t, true>() = [](void *p) {
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_U64, (uint64_t *)p);
    };
    GetVisualize<uint64_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_U64, (uint64_t *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<int64_t, true>() = [](void *p) {
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_S64, (int64_t *)p);
    };
    GetVisualize<int64_t, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_S64, (int64_t *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<float, true>() = [](void *p) {
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_Float, (float *)p);
    };
    GetVisualize<float, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_Float, (float *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<double, true>() = [](void *p) {
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_Double, (double *)p);
    };
    GetVisualize<double, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputScalar(("##" + to_string(uintptr_t(p))).c_str(), ImGuiDataType_Double, (double *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<string, true>() = [](void *p) {
        ImGui::InputText(("##" + to_string(uintptr_t(p))).c_str(), (string *)p);
    };
    GetVisualize<string, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputText(("##" + to_string(uintptr_t(p))).c_str(), (string *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<vec2, true>() = [](void *p) {
        ImGui::InputFloat2(("##" + to_string(uintptr_t(p))).c_str(), (float *)p);
    };
    GetVisualize<vec2, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputFloat2(("##" + to_string(uintptr_t(p))).c_str(), (float *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<vec3, true>() = [](void *p) {
        ImGui::InputFloat3(("##" + to_string(uintptr_t(p))).c_str(), (float *)p);
    };
    GetVisualize<vec3, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputFloat3(("##" + to_string(uintptr_t(p))).c_str(), (float *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<vec4, true>() = [](void *p) {
        ImGui::InputFloat4(("##" + to_string(uintptr_t(p))).c_str(), (float *)p);
    };
    GetVisualize<vec4, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputFloat4(("##" + to_string(uintptr_t(p))).c_str(), (float *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<mat2, true>() = [](void *p) {
        mat2 &v = *(mat2 *)p;
        ImGui::InputFloat2((": col0##" + to_string(uintptr_t(p))).c_str(), (float *)&v[0][0]);
        ImGui::InputFloat2((": col1##" + to_string(uintptr_t(p))).c_str(), (float *)&v[1][0]);
    };
    GetVisualize<mat2, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        mat2 &v = *(mat2 *)p;
        ImGui::InputFloat2((": col0##" + to_string(uintptr_t(p))).c_str(), (float *)&v[0][0]);
        ImGui::InputFloat2((": col1##" + to_string(uintptr_t(p))).c_str(), (float *)&v[1][0]);
        ImGui::PopDisabled();
    };
    GetVisualize<mat3, true>() = [](void *p) {
        mat3 &v = *(mat3 *)p;
        ImGui::InputFloat3((": col0##" + to_string(uintptr_t(p))).c_str(), (float *)&v[0][0]);
        ImGui::InputFloat3((": col1##" + to_string(uintptr_t(p))).c_str(), (float *)&v[1][0]);
        ImGui::InputFloat3((": col2##" + to_string(uintptr_t(p))).c_str(), (float *)&v[2][0]);
    };
    GetVisualize<mat3, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        mat3 &v = *(mat3 *)p;
        ImGui::InputFloat3((": col0##" + to_string(uintptr_t(p))).c_str(), (float *)&v[0][0]);
        ImGui::InputFloat3((": col1##" + to_string(uintptr_t(p))).c_str(), (float *)&v[1][0]);
        ImGui::InputFloat3((": col2##" + to_string(uintptr_t(p))).c_str(), (float *)&v[2][0]);
        ImGui::PopDisabled();
    };
    GetVisualize<mat4, true>() = [](void *p) {
        mat4 &v = *(mat4 *)p;
        ImGui::InputFloat4((": col0##" + to_string(uintptr_t(p))).c_str(), (float *)&v[0][0]);
        ImGui::InputFloat4((": col1##" + to_string(uintptr_t(p))).c_str(), (float *)&v[1][0]);
        ImGui::InputFloat4((": col2##" + to_string(uintptr_t(p))).c_str(), (float *)&v[2][0]);
        ImGui::InputFloat4((": col3##" + to_string(uintptr_t(p))).c_str(), (float *)&v[3][0]);
    };
    GetVisualize<mat4, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        mat4 &v = *(mat4 *)p;
        ImGui::InputFloat4((": col0##" + to_string(uintptr_t(p))).c_str(), (float *)&v[0][0]);
        ImGui::InputFloat4((": col1##" + to_string(uintptr_t(p))).c_str(), (float *)&v[1][0]);
        ImGui::InputFloat4((": col2##" + to_string(uintptr_t(p))).c_str(), (float *)&v[2][0]);
        ImGui::InputFloat4((": col3##" + to_string(uintptr_t(p))).c_str(), (float *)&v[3][0]);
        ImGui::PopDisabled();
    };
    GetVisualize<quat, true>() = [](void *p) {
        ImGui::InputFloat4(("##" + to_string(uintptr_t(p))).c_str(), (float *)p);
    };
    GetVisualize<quat, false>() = [](void *p) {
        ImGui::PushDisabled(true);
        ImGui::InputFloat4(("##" + to_string(uintptr_t(p))).c_str(), (float *)p);
        ImGui::PopDisabled();
    };
    GetVisualize<Entity *, true>() = [this](void *p) {
        pair<Type *, void *> &v = *(pair<Type *, void *> *)p;
        ShowIcon(v.first);
    };
    GetVisualize<Entity *, false>() = [](void *p) {
        ImGui::Button("const pointer");
    };
    GetVisualize<shared_ptr<Resource>, true>() = [](void *p) {
        ImGui::Button("shared pointer");
    };
    GetVisualize<shared_ptr<Resource>, false>() = [](void *p) {
        ImGui::Button("const shared pointer");
    };
    
    GetVisualize<pair, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("pair");
        pair<void *, void *> &v = *(pair<void *, void *> *)p;
        f[0](v.first);
        f[1](v.second);
    };
    GetVisualize<pair, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("const pair");
        pair<void *, void *> &v = *(pair<void *, void *> *)p;
        f[0](v.first);
        f[1](v.second);
    };
    GetVisualize<vector, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("vector");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
    GetVisualize<vector, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("const vector");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
    GetVisualize<list, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("list");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
    GetVisualize<list, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("const list");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
    GetVisualize<set, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("set");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
    GetVisualize<set, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("const set");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
    GetVisualize<unordered_set, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("unordered_set");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
    GetVisualize<unordered_set, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("const unordered_set");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
    GetVisualize<map, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("map");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
    GetVisualize<map, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("const map");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
    GetVisualize<unordered_map, true>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("unordered_map");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
    GetVisualize<unordered_map, false>() = [](const vector<function<void(void *)>> &f, void *p) {
        ImGui::Button("const unordererd_map");
        vector<void *> &v = *(vector<void *> *)p;
        for (void *e : v) {
            f[0](e);
        }
    };
}

void InspectorPanel::ShowContents() {
    Project &project = Project::GetInstance();
    if (!project.IsLoaded()) {
        ImGui::End();
        return;
    }

    Entity *entity = GetSelected();
    if (entity && entity->GetType() == GameObject::StaticType()) {
        GameObject *gameObject = (GameObject *)entity;
        ShowIcon(GameObject::StaticType());
        ImGui::SameLine();
        ImGui::Text(gameObject->GetName().c_str());
        for (Component *component : gameObject->GetAllComponents()) {
            ImGui::Separator();
            Type *type = component->GetType();
            ShowIcon(type);
            ImGui::SameLine();
            ImGui::Text(type->GetName().c_str());
            type->Visualize(component);
        }
    } else if (dynamic_cast<Asset *>(entity)){
        Asset *asset = (Asset *)entity;
        Type *type = asset->GetType();
        ShowIcon(type);
        ImGui::SameLine();
        ImGui::Text(asset->GetName().c_str());
        ImGui::Separator();
        type->Visualize(entity);
    }
}