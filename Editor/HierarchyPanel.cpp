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
        if (transform->IsRemoved()) {
            return;
        }

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (transform->GetChildren().empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }
        bool open = ImGui::TreeNodeEx((void*)(intptr_t)gameObject, flags, "");
        ImGui::SameLine();
        ImGui::Text(ICON_FA_CUBE);
        ImGui::SameLine();
        if (rename && selected == (void *)gameObject) {
            ShowRename(gameObject->GetName());
        } else {
            if (ImGui::Selectable((gameObject->GetName() + "##" + to_string((uint64_t)gameObject)).c_str(), (void *)gameObject == selected)) {
                selected = (void *)gameObject;
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