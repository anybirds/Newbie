#include <Project.hpp>
#include <Scene.hpp>

#include <imgui/imgui.h>

#include <SceneDialog.hpp>

using namespace std;
using namespace Engine;

void SceneDialog::CreateImGui() {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(480.0f, 0.0f));

    bool p_open = true;
    if (ImGui::BeginPopupModal("Open Scene", &p_open, ImGuiWindowFlags_NoResize)) {
        ImGui::BeginChild("", ImVec2(0.0f, 480.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
        Project &project = Project::GetInstance();
        int index = 0;
        for (auto &scene : project.GetAllScenes()) {
            if (ImGui::Selectable(scene.c_str(), selected == index, ImGuiSelectableFlags_AllowDoubleClick)) {
                if (ImGui::IsMouseDoubleClicked(0)) {
                    Scene::Load(scene);
                    ImGui::CloseCurrentPopup();
                }
                selected = index;
                arg = scene;
            }
            index++;
        }
        ImGui::EndChild();

        ImGui::Separator();
        ImGui::Indent(ImGui::GetWindowWidth() - 95.0f);
        if (ImGui::Button("Select", ImVec2(80.0f, 0.0f))) {
            if (selected >= 0) {
                Scene::Load(arg);
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}