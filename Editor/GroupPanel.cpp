#include <string>
#include <iostream>

#include <imgui/imgui.h>
#include <Icons/IconsFontAwesome5.h>

#include <GroupPanel.hpp>
#include <Scene.hpp>
#include <Group.hpp>

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

        unsigned cnt = 0;
        for (auto it = scene.GetAllGroups().begin(); it != scene.GetAllGroups().end(); ) {
            Group *group = *it;
            bool p_open = true;
            if (ImGui::BeginTabItem((group->GetName() + "##" + to_string(cnt)).c_str(), &p_open, ImGuiTabItemFlags_None)) {

                ImGui::EndTabItem();
            }
            if (p_open) {
                it++; cnt++;
            } else {
                it = scene.RemoveGroup(it);
            }
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}