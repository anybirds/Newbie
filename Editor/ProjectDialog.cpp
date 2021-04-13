#include <filesystem>
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

#include <Project.hpp>
#include <ProjectDialog.hpp>
#include <FileDialog.hpp>

using namespace std;
using namespace Engine;

void ProjectDialog::CreateImGui() {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(480.0f, 0.0f));

    bool p_open = true;
    if (ImGui::BeginPopupModal("New Project", &p_open, ImGuiWindowFlags_NoResize)) {
        ImGui::Text("Name :");
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
                auto sample(filesystem::u8path(string(NEWBIE_PATH) + "/Samples/Empty"));
                auto created(filesystem::u8path(location + "/" + name));
                try {
                    // copy files from "Samples/Empty"
                    filesystem::copy(sample, created);

                    // rename project file
                    filesystem::path proj;
                    for (auto &p : filesystem::directory_iterator(created)) {
                        // find Empty.json file
                        if (p.path().filename().string() == "Empty.json") {
                            proj = p;
                            proj.replace_filename(filesystem::u8path(name + ".json"));
                            filesystem::rename(p, proj);
                            break;
                        }
                    }

                    // load project
                    Project &project = Project::GetInstance();
                    project.Load(proj.u8string());

                    name.clear();
                    location.clear();
                    ImGui::CloseCurrentPopup();
                } catch (...) {}
            }
        }

        ImGui::EndPopup();
    }
}