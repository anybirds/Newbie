#include <imgui/imgui.h>

#include <Project.hpp>
#include <Scene.hpp>
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
        for (auto &path : project.GetAllScenes()) {
            if (ImGui::Selectable(path.c_str(), selected == index, ImGuiSelectableFlags_AllowDoubleClick)) {
                if (ImGui::IsMouseDoubleClicked(0)) {
                    Scene &scene = Scene::GetInstance();
                    scene.Load(path);
                    selected = -1;
                    ImGui::CloseCurrentPopup();
                } else {
                    selected = index;
                }
                arg = path;
            }
            index++;
        }
        ImGui::EndChild();

        ImGui::Separator();
        ImGui::Indent(ImGui::GetWindowWidth() - 95.0f);
        if (ImGui::Button("Select", ImVec2(80.0f, 0.0f))) {
            if (selected >= 0) {
                Scene &scene = Scene::GetInstance();
                scene.Load(arg);
                selected = -1;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}