#include <cstdint>
#include <functional>

#include <HierarchyPanel.hpp>
#include <Scene.hpp>
#include <Transform.hpp>
#include <Prefab.hpp>

using namespace std;

void HierarchyPanel::ShowContents() {
    Scene &scene = Scene::GetInstance();
    if (!scene.IsLoaded()) {
        ImGui::End();
        return;
    }

    GameObject *root = scene.GetRoot();
    GameObject *hovered = nullptr;
    Entity *source = nullptr;
    GameObject *sibling = nullptr;
    GameObject *parent = nullptr;
    function<void(GameObject *)> recurse = [&](GameObject *gameObject) {
        Transform *transform = gameObject->GetTransform();

        ImGui::InvisibleButton(("##" + to_string((uintptr_t)gameObject)).c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 3.0f));
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset")) {
                IM_ASSERT(payload->DataSize == sizeof(Asset *));
                source = *(Entity **)payload->Data;
                if (source->GetType() == APrefab::StaticType()) { 
                    sibling = gameObject;
                }
            }
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
                IM_ASSERT(payload->DataSize == sizeof(GameObject *));
                source = *(Entity **)payload->Data;
                sibling = gameObject;
            }
            ImGui::EndDragDropTarget();
        }
        
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (transform->GetChildren().empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }
        bool open = ImGui::TreeNodeEx((void*)(intptr_t)gameObject, flags, "");
        ImGui::SameLine();
        ImGui::Text(ICON_FA_CUBE);
        ImGui::SameLine();
        if (IsRenaming() && !GetLocalSelected() && GetSelected() == (void *)gameObject) {
            ShowRenamingItem(gameObject->GetName());
        } else {
            ImGui::Selectable((gameObject->GetName() + "##" + to_string((uintptr_t)gameObject)).c_str(), (void *)gameObject == GetSelected());
            if (ImGui::IsItemHovered()) {
                hovered = gameObject;
                if (ImGui::IsMouseDown(0)) {
                    GetSelected() = (Entity *)gameObject;
                }
            }
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                ImGui::SetDragDropPayload("GameObject", &GetSelected(), sizeof(GameObject *));
                ImGui::Text(ICON_FA_CUBE);
                ImGui::SameLine();
                ImGui::Text(((GameObject *)GetSelected())->GetName().c_str());
                ImGui::EndDragDropSource();
            }
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset")) {
                    IM_ASSERT(payload->DataSize == sizeof(Asset *));
                    source = *(Entity **)payload->Data;
                    if (source->GetType() == APrefab::StaticType()) { 
                        parent = gameObject;
                    }
                }
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
                    IM_ASSERT(payload->DataSize == sizeof(GameObject *));
                    source = *(Entity **)payload->Data;
                    parent = gameObject;
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
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, 0.0f));
    recurse(root);
    ImGui::PopStyleVar();
    
    Asset *asset;
    GameObject *gameObject;
    if (parent) {
        if (source->GetType() == APrefab::StaticType()) {
            asset = (Asset *)source;
            parent->AddGameObject(dynamic_pointer_cast<Prefab>(asset->GetResource()));
        } else {
            gameObject = (GameObject *)source;
            gameObject->SetParent(parent);
        }
    }
    if (sibling) {
        if (source->GetType() == APrefab::StaticType()) {
            asset = (Asset *)source;
            gameObject = root->AddGameObject(dynamic_pointer_cast<Prefab>(asset->GetResource()));
            gameObject->SetSibling(sibling);
        } else {
            gameObject = (GameObject *)source;
            gameObject->SetSibling(sibling);
        }
    }

    if (ImGui::BeginPopupContextWindow())
    {
        if (!menu) {
            GetSelected() = (Entity *)hovered;
        }
        gameObject = (GameObject *)GetSelected();
        Scene &scene = Scene::GetInstance();
        if (ImGui::MenuItem("Cut", nullptr, nullptr, (bool)gameObject)) {
            GameObject::ToJson(copyed, gameObject);
            gameObject->Remove();
        }
        if (ImGui::MenuItem("Copy", nullptr, nullptr, (bool)gameObject)) {
            GameObject::ToJson(copyed, gameObject);
        }
        if (ImGui::MenuItem("Paste", nullptr, nullptr, !copyed.empty())) {
            if (gameObject) {
                gameObject->AddGameObject(copyed);
            } else {
                root->AddGameObject(copyed);
            }
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Add GameObject", nullptr, nullptr)) {
            if (gameObject) {
                gameObject->AddGameObject();
            } else {
                root->AddGameObject();
            }
        }
        if (ImGui::MenuItem("Remove", nullptr, nullptr, (bool)gameObject)) {
            gameObject->Remove();
        }
        
        menu = true;
        ImGui::EndPopup();
    } else {
        menu = false;
    }
    ImGui::EndChild();
    if (ImGui::BeginDragDropTarget() && !hovered) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset")) {
            IM_ASSERT(payload->DataSize == sizeof(Asset *));
            Asset *asset = *(Asset **)payload->Data;
            if (asset->GetType() == APrefab::StaticType()) {
                root->AddGameObject(dynamic_pointer_cast<Prefab>(asset->GetResource()));
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void HierarchyPanel::Clear() {
    copyed.clear();
}