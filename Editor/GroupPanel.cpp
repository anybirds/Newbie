#include <string>
#include <functional>

#include <imgui/imgui.h>
#include <Icons/IconsFontAwesome5.h>

#include <GroupPanel.hpp>
#include <Scene.hpp>
#include <Group.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>

using namespace std;
using namespace Engine;

void GroupPanel::CreateImGui() {
    if (!open) {
        return;
    }

    ImGui::Begin("Group", &open);

    Scene &scene = Scene::GetInstance();
    if (!scene.IsLoaded()) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabBar("Group", 
            ImGuiTabBarFlags_Reorderable | 
            ImGuiTabBarFlags_AutoSelectNewTabs | 
            ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | 
            ImGuiTabBarFlags_FittingPolicyResizeDown)) {
        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
            scene.AddGroup();
        }

        function<void(GameObject *)> recurse = [&recurse](GameObject *gameObject) {
            Transform *transform = gameObject->GetTransform();
            static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
            if (transform->GetChildren().empty()) {
                ImGui::TreeNodeEx((void*)(intptr_t)gameObject,
                    base_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, 
                    (string(ICON_FA_CUBE) + gameObject->GetName()).c_str());
            } else if (ImGui::TreeNodeEx((void*)(intptr_t)gameObject, base_flags, (string(ICON_FA_CUBE) + gameObject->GetName()).c_str())) {
                for (Transform *t : transform->GetChildren()) {
                    recurse(t->GetGameObject());
                }
                ImGui::TreePop();
            }
        };
        unsigned index = 0;
        Group *g = nullptr;
        for (Group *group : scene.GetAllGroups()) {
            bool p_open = true;
            if (ImGui::BeginTabItem((group->GetName() + "##" + to_string(index)).c_str(), &p_open, ImGuiTabItemFlags_None)) {
                ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
                for (GameObject *gameObject : group->GetRootGameObjects()) {
                    recurse(gameObject);
                }
                ImGui::EndTabItem();
            }
            if (p_open) {
                index++;
            } else {
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