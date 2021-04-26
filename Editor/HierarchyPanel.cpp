#include <cstdint>
#include <functional>

#include <HierarchyPanel.hpp>
#include <Scene.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>

using namespace std;
using namespace Engine;

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
        if (gameObject->GetPrefab()) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
            ImGui::Text(ICON_FA_CUBE);
            ImGui::PopStyleColor();
        } else {
            ImGui::Text(ICON_FA_CUBE);
        }
        ImGui::SameLine();
        if (rename && selected == (void *)gameObject) {
            ShowRename(gameObject->GetName());
        } else {
            ImGui::Selectable((gameObject->GetName() + "##" + to_string((uint64_t)gameObject)).c_str(), (void *)gameObject == selected);
            if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0)) {
                selected = (void *)gameObject;
            }
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                ImGui::SetDragDropPayload("GameObject", &selected, sizeof(GameObject *));
                ImGui::Text(ICON_FA_CUBE);
                ImGui::SameLine();
                ImGui::Text(((GameObject *)selected)->GetName().c_str());
                ImGui::EndDragDropSource();
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
    
    for (GameObject *root : scene.GetRootGameObjects()) {
        recurse(root);
    }

    ImGui::End();
}