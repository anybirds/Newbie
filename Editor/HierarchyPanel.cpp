#include <cstdint>
#include <functional>

#include <HierarchyPanel.hpp>
#include <Scene.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Prefab.hpp>
#include <Graphics/Window.hpp>

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

        ImGui::PushID(gameObject);
        ImGui::InvisibleButton("##", ImVec2(ImGui::GetContentRegionAvailWidth(), 3.0f));
        ImGui::PopID();
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
        bool open = ImGui::TreeNodeEx((const void*)transform, flags, "");
        ImGui::SameLine();
        ShowIcon(GameObject::StaticType());
        ImGui::SameLine();
        if (IsRenaming() && !GetLocalSelected() && GetSelected() == (void *)gameObject) {
            string name = gameObject->GetName();
            ShowRenamingItem(name);
            gameObject->SetName(name);
        } else {
            ImGui::PushID(gameObject);
            if (ImGui::Selectable(gameObject->GetName().c_str(), (void *)gameObject == GetSelected())) {
                GetSelected() = (Entity *)gameObject;
            }
            ImGui::PopID();
            if (ImGui::IsItemHovered()) {
                hovered = gameObject;
            }
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                static GameObject *source;
                source = gameObject;
                ImGui::SetDragDropPayload("GameObject", &source, sizeof(GameObject *));
                ShowIcon(GameObject::StaticType());
                ImGui::SameLine();
                ImGui::Text(source->GetName().c_str());
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
    
    ImGui::BeginChild("Hierarchy", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);

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

    static bool menu;
    if (ImGui::BeginPopupContextWindow())
    {
        if (!menu) {
            GetSelected() = (Entity *)hovered;
        }
        gameObject = (GameObject *)GetSelected();
        if (ImGui::MenuItem("Cut", nullptr, nullptr, (bool)gameObject)) {
            GetCopyedType() = GameObject::StaticType();
            GameObject::ToJson(GetCopyed(), gameObject);
            gameObject->Remove();
            GetSelected() = nullptr;
        }
        if (ImGui::MenuItem("Copy", nullptr, nullptr, (bool)gameObject)) {
            GetCopyedType() = GameObject::StaticType();
            GameObject::ToJson(GetCopyed(), gameObject);
        }
        if (ImGui::MenuItem("Paste", nullptr, nullptr, GetCopyedType() == GameObject::StaticType() && !GetCopyed().empty())) {
            if (gameObject) {
                GetSelected() = gameObject->AddGameObject(GetCopyed());
            } else {
                GetSelected() = root->AddGameObject(GetCopyed());
            }
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Add GameObject", nullptr, nullptr)) {
            if (gameObject) {
                GetSelected() = gameObject->AddGameObject();
            } else {
                GetSelected() = root->AddGameObject();
            }
        }
        if (ImGui::MenuItem("Remove", nullptr, nullptr, (bool)gameObject)) {
            gameObject->Remove();
            GetSelected() = nullptr;
        }
        
        menu = true;
        ImGui::EndPopup();
    } else {
        menu = false;
    }

    if (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
        if (ImGui::IsKeyPressed(GLFW_KEY_X)) {
            if (GetSelected() && GetSelected()->GetType() == GameObject::StaticType()) {
                gameObject = (GameObject *)GetSelected();
                GetCopyedType() = GameObject::StaticType();
                GameObject::ToJson(GetCopyed(), gameObject);
                gameObject->Remove();
                GetSelected() = nullptr;
            }
        } else if (ImGui::IsKeyPressed(GLFW_KEY_C)) {
            if (GetSelected() && GetSelected()->GetType() == GameObject::StaticType()) {
                gameObject = (GameObject *)GetSelected();
                GetCopyedType() = GameObject::StaticType();
                GameObject::ToJson(GetCopyed(), gameObject);
            }
        } else if (ImGui::IsKeyPressed(GLFW_KEY_V)) {
            if (GetCopyedType() == GameObject::StaticType()) {
                if (GetSelected() && GetSelected()->GetType() == GameObject::StaticType()) {
                    gameObject = (GameObject *)GetSelected();
                    GetSelected() = gameObject->AddGameObject(GetCopyed());
                } else {
                    GetSelected() = root->AddGameObject(GetCopyed());
                }
            }
        }
    }
    if (ImGui::IsKeyPressed(GLFW_KEY_DELETE)) {
        if (GetSelected() && GetSelected()->GetType() == GameObject::StaticType()) {
            gameObject = (GameObject *)GetSelected();
            gameObject->Remove();
            GetSelected() = nullptr;
        }
    }

    ImGui::EndChild();

    if (ImGui::BeginDragDropTarget() && !hovered) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset")) {
            IM_ASSERT(payload->DataSize == sizeof(Asset *));
            Asset *asset = *(Asset **)payload->Data;
            if (asset->GetType() == APrefab::StaticType()) {
                GetSelected() = root->AddGameObject(dynamic_pointer_cast<Prefab>(asset->GetResource()));
            }
        }
        ImGui::EndDragDropTarget();
    }
}