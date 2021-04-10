#include <Project.hpp>

#include <filesystem>
#include <string>

#include <imgui/imgui.h>

#include <MainMenuBar.hpp>
#include <FileDialog.hpp>
#include <SceneDialog.hpp>

using namespace std;
using namespace Engine;

void MainMenuBar::CreateImGui() {
    if (ImGui::BeginMainMenuBar())
    {
        bool project_open = false;
        bool scene_open = false;
        if (ImGui::BeginMenu("Project"))
        {
            if (ImGui::MenuItem("New")) {

            }
            if (ImGui::MenuItem("Open")) {
                project_open = true;
            }
            if (ImGui::MenuItem("Save")) {
                
            }
            ImGui::EndMenu();
        }
        Project &project = Project::GetInstance();
        if (ImGui::BeginMenu("Scene", project.IsLoaded())) {
            if (ImGui::MenuItem("New")) {

            }
            if (ImGui::MenuItem("Open")) {
                scene_open = true;
            }
            ImGui::EndMenu();
        }

        FileDialog &fileDialog = FileDialog::GetInstance();
        if (project_open) {
            fileDialog.SetFolderDialog(true);
            fileDialog.SetCallback([](const string &path) {
                // find the project file 
                string pfile;
                for (auto &p : filesystem::directory_iterator(path)) {
                    if (p.path().extension().string() == ".json") {
                        pfile = p.path().string();
                        break;
                    }
                }

                // load the Project
                Project &project = Project::GetInstance();
                if (!project.Load(pfile)) {
                    return;
                }
                project.SetUseDefaultFramebuffer(false); // use game panel framebuffer
            });
            ImGui::OpenPopup("Open Folder");
        }
        fileDialog.CreateImGui();

        SceneDialog &sceneDialog = SceneDialog::GetInstance();
        if (scene_open) {
            ImGui::OpenPopup("Open Scene");
        }
        sceneDialog.CreateImGui();

        ImGui::EndMainMenuBar();
    }
}
