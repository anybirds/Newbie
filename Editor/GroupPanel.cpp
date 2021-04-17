#include <cstdint>
#include <functional>

#include <GroupPanel.hpp>
#include <Scene.hpp>
#include <Transform.hpp>

using namespace std;
using namespace Engine;

void GroupPanel::ShowContents() {
    Scene &scene = Scene::GetInstance();
    if (!scene.IsLoaded()) {
        ImGui::End();
        return;
    }
    
    for (Group *group : scene.GetAllGroups()) {
        if (rename && selected == (void *)group) {
            ShowRename(group->GetName());
        }
    }

    if (ImGui::BeginTabBar("Group", 
            ImGuiTabBarFlags_Reorderable | 
            ImGuiTabBarFlags_AutoSelectNewTabs | 
            ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | 
            ImGuiTabBarFlags_FittingPolicyResizeDown)) {
        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
            Group *group = scene.AddGroup();
            group->SetName("New Group");
            selected = group;
            rename = true;
        }
        
        function<void(GameObject *)> recurse = [&recurse, this](GameObject *gameObject) {
            Transform *transform = gameObject->GetTransform();
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            if (transform->GetChildren().empty()) {
                flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            }
            if (ImGui::TreeNodeEx((void*)(intptr_t)gameObject, flags, "")) {
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

                for (Transform *t : transform->GetChildren()) {
                    recurse(t->GetGameObject());
                }

                if (!transform->GetChildren().empty()) {
                    ImGui::TreePop();
                }
            }
        };
        
        Group *g = nullptr;
        for (Group *group : scene.GetAllGroups()) {
            bool p_open = true;
            if (ImGui::BeginTabItem((group->GetName() + "##" + to_string((uint64_t)group)).c_str(), &p_open, ImGuiTabItemFlags_None)) {
                if (ImGui::IsItemClicked()) {
                    selected = (void *)group;
                }
                for (GameObject *gameObject : group->GetRootGameObjects()) {
                    recurse(gameObject);
                }
                ImGui::EndTabItem();
            }
            if (!p_open) {
                g = group;
            }
        }
        if (g) {
            scene.RemoveGroup(g);
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}