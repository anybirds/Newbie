#include <Engine.hpp>
#include <InspectorPanel.hpp>
#include <AssetPanel.hpp>

using namespace std;
using namespace glm;

InspectorPanel::InspectorPanel() : Panel("Inspector") {
    GetVisualizeProperty() = [](const string &name, const function<void(void *)> &visualize, void *pointer) {
        if (ImGui::BeginTable(name.c_str(), 2, ImGuiTableFlags_None)) {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, ImGui::GetContentRegionAvail().x * 0.2f);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
            
            ImGui::TableNextColumn();
            ImGui::Text(name.c_str());
            ImGui::TableNextColumn();
            visualize(pointer);
            ImGui::EndTable();
        }
    };
    GetVisualize<bool, true>() = [](void *p) {
        ImGui::Button("bool");
    };
    GetVisualize<bool, false>() = [](void *p) {
        ImGui::Button("const bool");
    };
    GetVisualize<uint8_t, true>() = [](void *p) {
        ImGui::Button("uint8_t");
    };
    GetVisualize<uint8_t, false>() = [](void *p) {
        ImGui::Button("const uint8_t");
    };
    GetVisualize<int8_t, true>() = [](void *p) {
        ImGui::Button("int8_t");
    };
    GetVisualize<int8_t, false>() = [](void *p) {
        ImGui::Button("const int8_t");
    };
    GetVisualize<uint16_t, true>() = [](void *p) {
        ImGui::Button("uint16_t");
    };
    GetVisualize<uint16_t, false>() = [](void *p) {
        ImGui::Button("const uint16_t");
    };
    GetVisualize<int16_t, true>() = [](void *p) {
        ImGui::Button("int16_t");
    };
    GetVisualize<int16_t, false>() = [](void *p) {
        ImGui::Button("const int16_t");
    };
    GetVisualize<uint32_t, true>() = [](void *p) {
        ImGui::Button("uint32_t");
    };
    GetVisualize<uint32_t, false>() = [](void *p) {
        ImGui::Button("const uint32_t");
    };
    GetVisualize<int32_t, true>() = [](void *p) {
        ImGui::Button("int32_t");
    };
    GetVisualize<int32_t, false>() = [](void *p) {
        ImGui::Button("const int32_t");
    };
    GetVisualize<uint64_t, true>() = [](void *p) {
        ImGui::Button("uint64_t");
    };
    GetVisualize<uint64_t, false>() = [](void *p) {
        ImGui::Button("const uint64_t");
    };
    GetVisualize<int64_t, true>() = [](void *p) {
        ImGui::Button("int64_t");
    };
    GetVisualize<int64_t, false>() = [](void *p) {
        ImGui::Button("const int64_t");
    };
    GetVisualize<float, true>() = [](void *p) {
        ImGui::Button("float");
    };
    GetVisualize<float, false>() = [](void *p) {
        ImGui::Button("const float");
    };
    GetVisualize<double, true>() = [](void *p) {
        ImGui::Button("double");
    };
    GetVisualize<double, false>() = [](void *p) {
        ImGui::Button("const double");
    };
    GetVisualize<string, true>() = [](void *p) {
        ImGui::Button("string");
    };
    GetVisualize<string, false>() = [](void *p) {
        ImGui::Button("string");
    };
    GetVisualize<vec2, true>() = [](void *p) {
        ImGui::Button("vec2");
    };
    GetVisualize<vec2, false>() = [](void *p) {
        ImGui::Button("const vec2");
    };
    GetVisualize<vec3, true>() = [](void *p) {
        ImGui::Button("vec3");
    };
    GetVisualize<vec3, false>() = [](void *p) {
        ImGui::Button("const vec3");
    };
    GetVisualize<vec4, true>() = [](void *p) {
        ImGui::Button("vec4");
    };
    GetVisualize<vec4, false>() = [](void *p) {
        ImGui::Button("const vec4");
    };
    GetVisualize<mat2, true>() = [](void *p) {
        ImGui::Button("mat2");
    };
    GetVisualize<mat2, false>() = [](void *p) {
        ImGui::Button("const mat2");
    };
    GetVisualize<mat3, true>() = [](void *p) {
        ImGui::Button("mat3");
    };
    GetVisualize<mat4, false>() = [](void *p) {
        ImGui::Button("const mat4");
    };
    GetVisualize<quat, true>() = [](void *p) {
        ImGui::Button("quat");
    };
    GetVisualize<quat, false>() = [](void *p) {
        ImGui::Button("const quat");
    };
    GetVisualize<Entity *, true>() = [](void *p) {
        ImGui::Button("pointer");
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
        ImGui::Text(ICON_FA_CUBE);
        ImGui::SameLine();
        ImGui::Text(gameObject->GetName().c_str());
        for (Component *component : gameObject->GetAllComponents()) {
            ImGui::Separator();
            Type *type = component->GetType();
            ShowIcon(component);
            ImGui::SameLine();
            ImGui::Text(type->GetName().c_str());
            type->Visualize(component);
        }
    } else if (dynamic_cast<Asset *>(entity)){
        Asset *asset = (Asset *)entity;
        ShowIcon(asset);
        ImGui::SameLine();
        ImGui::Text(asset->GetName().c_str());
        ImGui::Separator();
        asset->GetType()->Visualize(entity);
    }
}