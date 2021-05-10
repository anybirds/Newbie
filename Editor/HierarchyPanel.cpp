#include <cstdint>
#include <functional>

#include <HierarchyPanel.hpp>
#include <Scene.hpp>
#include <Transform.hpp>
#include <Prefab.hpp>

using namespace std;

void HierarchyPanel::ShowIcon(GameObject *gameObject) {
    if (gameObject->GetPrefab()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
        ImGui::Text(ICON_FA_CUBE);
        ImGui::PopStyleColor();
    } else {
        ImGui::Text(ICON_FA_CUBE);
    }
}

void HierarchyPanel::ShowContents() {
    Scene &scene = Scene::GetInstance();
    if (!scene.IsLoaded()) {
        ImGui::End();
        return;
    }
        
    function<void(GameObject *)> recurse = [&recurse, this](GameObject *gameObject) {
        Transform *transform = gameObject->GetTransform();

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (transform->GetChildren().empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }
        bool open = ImGui::TreeNodeEx((void*)(intptr_t)gameObject, flags, "");
        ImGui::SameLine();
        ShowIcon(gameObject);
        ImGui::SameLine();
        if (rename && selected == (void *)gameObject) {
            ShowRename(gameObject->GetName());
        } else {
            ImGui::Selectable((gameObject->GetName() + "##" + to_string((uintptr_t)gameObject)).c_str(), (void *)gameObject == selected);
            if (ImGui::IsItemHovered()) {
                hovered = gameObject;
                if (ImGui::IsMouseDown(0)) {
                    selected = (void *)gameObject;
                }
            }
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                ImGui::SetDragDropPayload("GameObject", &selected, sizeof(GameObject *));
                ShowIcon((GameObject *)selected);
                ImGui::SameLine();
                ImGui::Text(((GameObject *)selected)->GetName().c_str());
                ImGui::EndDragDropSource();
            }
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset")) {
                    IM_ASSERT(payload->DataSize == sizeof(Asset *));
                    Asset *asset = *(Asset **)payload->Data;
                    if (asset->GetType() == APrefab::StaticType()) {
                        gameObject->AddGameObject(dynamic_pointer_cast<Prefab>(asset->GetResource()));
                    }
                }
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
                    IM_ASSERT(payload->DataSize == sizeof(GameObject *));
                    GameObject *child = *(GameObject **)payload->Data;
                    child->SetParent(gameObject);
                }
                ImGui::EndDragDropTarget();
            }
        }
        if (open) {
            for (Transform *t : transform->GetChildren()) {
                recurse(t->GetGameObject());
            }

            if (!transform->GetChildren().empty()) {
                ImGui::TreePop();
            }
        }
    };
    
    ImGui::BeginChild("Asset", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    hovered = nullptr;
    for (GameObject *root : scene.GetRootGameObjects()) {
        recurse(root);
    }
    if (ImGui::BeginPopupContextWindow())
    {
        if (!menu) {
            selected = (void *)hovered;
        }
        Scene &scene = Scene::GetInstance();
        if (ImGui::MenuItem("Cut", nullptr, nullptr, (bool)selected)) {
            GameObject *gameObject = (GameObject *)selected;
            if (copyed) {
                scene.DestroyGameObject(copyed);
            }
            copyed = gameObject->GetCopy();
            scene.RemoveGameObject(gameObject);
        }
        if (ImGui::MenuItem("Copy", nullptr, nullptr, (bool)selected)) {
            GameObject *gameObject = (GameObject *)selected;
            if (copyed) {
                scene.DestroyGameObject(copyed);
            }
            copyed = gameObject->GetCopy();
        }
        if (ImGui::MenuItem("Paste", nullptr, nullptr, (bool)copyed && (selected || !preview))) {
            if (selected) {
                GameObject *gameObject = (GameObject *)selected;
                GameObject *child = gameObject->AddGameObject(copyed);
            } else {
                scene.AddGameObject(copyed);
            }
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Add GameObject", nullptr, nullptr, selected || !preview)) {
            if (selected) {
                GameObject *gameObject = (GameObject *)selected;
                GameObject *child = gameObject->AddGameObject();
                child->SetName("GameObject");
            } else {
                GameObject *root = scene.AddGameObject();
                root->SetName("GameObject");
            }
        }
        if (ImGui::MenuItem("Remove", nullptr, nullptr, (bool)selected)) {
            GameObject *gameObject = (GameObject *)selected;
            scene.RemoveGameObject(gameObject);
        }
        
        menu = true;
        ImGui::EndPopup();
    } else {
        menu = false;
    }
    ImGui::EndChild();
    if (ImGui::BeginDragDropTarget() && !hovered && !preview) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset")) {
            IM_ASSERT(payload->DataSize == sizeof(Asset *));
            Asset *asset = *(Asset **)payload->Data;
            if (asset->GetType() == APrefab::StaticType()) {
                scene.AddGameObject(dynamic_pointer_cast<Prefab>(asset->GetResource()));
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void HierarchyPanel::Clear() {
    if (copyed) {
        Scene::GetInstance().DestroyGameObject(copyed);
    }
    copyed = nullptr;
}