#include <cstdint>
#include <string>
#include <functional>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <Icons/IconsFontAwesome5.h>

#include <GroupPanel.hpp>
#include <Scene.hpp>
#include <Transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
            selectedGroup = scene.AddGroup();
            renamedGroup = selectedGroup;
            rename = "New Group";
        }
        
        function<void(GameObject *)> recurse = [&recurse, this](GameObject *gameObject) {
            Transform *transform = gameObject->GetTransform();
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            if (transform->GetChildren().empty()) {
                flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            }
            bool clicked = false;
            if (ImGui::TreeNodeEx((void*)(intptr_t)gameObject, flags, "")) {
                ImGui::SameLine();
                ImGui::Text(ICON_FA_CUBE);
                ImGui::SameLine();
                if (renamedGameObject == gameObject) {
                    if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
                        ImGui::SetKeyboardFocusHere();
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
                    ImGui::InputText((string("##") + to_string((uint64_t)gameObject)).c_str(), &rename, ImGuiInputTextFlags_AutoSelectAll);
                    ImGui::PopStyleColor();
                    ImGui::PopStyleVar();
                } else {
                    if (ImGui::Selectable((gameObject->GetName() + "##" + to_string((uint64_t)gameObject)).c_str(), gameObject == selectedGameObject)) {
                        clicked = true;
                    }
                }
                if (clicked) {
                    selectedGameObject = gameObject;
                    selectedGroup = nullptr;
                }

                for (Transform *t : transform->GetChildren()) {
                    recurse(t->GetGameObject());
                }

                if (!transform->GetChildren().empty()) {
                    ImGui::TreePop();
                }
            }
        };
        
        unsigned index = 0;
        Group *g = nullptr;
        for (Group *group : scene.GetAllGroups()) {
            bool p_open = true;
            if (ImGui::BeginTabItem((group->GetName() + "##" + to_string(index)).c_str(), &p_open, ImGuiTabItemFlags_None)) {
                if (ImGui::IsItemClicked()) {
                    selectedGroup = group;
                    selectedGameObject = nullptr;
                }   
                if (renamedGroup == group) {
                    if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
                        ImGui::SetKeyboardFocusHere();
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
                    ImGui::InputText((string("##") + to_string((uint64_t)group)).c_str(), &rename, ImGuiInputTextFlags_AutoSelectAll);
                    ImGui::PopStyleColor();
                    ImGui::PopStyleVar();
                }
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
        if (selectedGroup && ImGui::IsKeyPressed(GLFW_KEY_F2)) {
            renamedGroup = selectedGroup;
            rename = renamedGroup->GetName();
        }
        if (selectedGameObject && ImGui::IsKeyPressed(GLFW_KEY_F2)) {
            renamedGameObject = selectedGameObject;
            rename = renamedGameObject->GetName();
        }
        if (renamedGameObject && (ImGui::IsKeyPressed(GLFW_KEY_ENTER) || ImGui::IsMouseClicked(0))) {
            renamedGameObject->SetName(rename);
            renamedGameObject = nullptr;
        }
        if (renamedGroup && (ImGui::IsKeyPressed(GLFW_KEY_ENTER) || ImGui::IsMouseClicked(0))) {
            renamedGroup->SetName(rename);
            renamedGroup = nullptr;
        }
        if (g) {
            scene.RemoveGroup(g);
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}