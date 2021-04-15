#include <filesystem>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <Icons/IconsFontAwesome5.h>

#include <Project.hpp>
#include <Scene.hpp>
#include <SceneDialog.hpp>
#include <FileDialog.hpp>

using namespace std;
using namespace Engine;

void SceneDialog::CreateImGui() {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(480.0f, 0.0f));

    bool p_open = true;
    if (ImGui::BeginPopupModal("New Scene", &p_open, ImGuiWindowFlags_NoResize)) {
        ImGui::Text("File Name :");
        ImGui::InputText("##Name", &name);
        
        ImGui::Text("Location :");
        ImGui::InputText("##Location", &location);
        ImGui::SameLine();
        FileDialog &fileDialog = FileDialog::GetInstance();
        if (ImGui::Button("Browse...")) {
            fileDialog.SetFolderDialog(true);
            fileDialog.SetCallback([this](const string &path) {
                this->location = path;
            });
            ImGui::OpenPopup("Open Folder");
        }
        fileDialog.CreateImGui();

        ImGui::Separator();
        ImGui::Indent(ImGui::GetWindowWidth() - 95.0f);
        if (ImGui::Button("Create", ImVec2(80.0f, 0.0f))) {
            if (!name.empty() && !location.empty()) {
                auto sample(filesystem::u8path(string(NEWBIE_PATH) + "/Samples/Empty/EmptyScene.json"));
                auto created(filesystem::u8path(location + "/" + name));
                try {
                    // copy file "Samples/Empty/EmptyScene.json"
                    filesystem::copy_file(sample, created);

                    // add scene
                    Project &project = Project::GetInstance();
                    project.AddScene(filesystem::relative(created, filesystem::u8path(project.GetDirectoy())).u8string());

                    name.clear();
                    location.clear();
                    ImGui::CloseCurrentPopup();
                } catch (...) {}
            }
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Open Scene", &p_open, ImGuiWindowFlags_NoResize)) {
        ImGui::BeginChild("", ImVec2(0.0f, 480.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
        Project &project = Project::GetInstance();
        int index = 0;
        for (auto &path : project.GetAllScenes()) {
            if (ImGui::Button(ICON_FA_TIMES)) {
                project.RemoveScene(path);
            }
            ImGui::SameLine();
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
        FileDialog &fileDialog = FileDialog::GetInstance();
        if (ImGui::Button("Add...")) {
            fileDialog.SetFolderDialog(false);
            fileDialog.SetCallback([](const string &path) {
                Project &project = Project::GetInstance();
                project.AddScene(filesystem::relative(path, filesystem::u8path(project.GetDirectoy())).u8string());
            });
            ImGui::OpenPopup("Open File");
        }
        fileDialog.CreateImGui();
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