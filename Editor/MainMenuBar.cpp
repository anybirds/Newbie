#include <filesystem>
#include <string>

#include <imgui/imgui.h>

#include <Project.hpp>
#include <MainMenuBar.hpp>
#include <FileDialog.hpp>
#include <SceneDialog.hpp>
#include <ProjectDialog.hpp>
#include <GamePanel.hpp>
#include <ScenePanel.hpp>

using namespace std;
using namespace Engine;

void MainMenuBar::CreateImGui() {
    if (ImGui::BeginMainMenuBar())
    {
        bool project_new = false;
        bool project_open = false;
        bool scene_open = false;
        Project &project = Project::GetInstance();
        if (ImGui::BeginMenu("Project"))
        {
            if (ImGui::MenuItem("New")) {
                project_new = true;
            }
            if (ImGui::MenuItem("Open")) {
                project_open = true;
            }
            if (ImGui::MenuItem("Save", nullptr, nullptr, project.IsLoaded())) {
                project.Save();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene", project.IsLoaded())) {
            if (ImGui::MenuItem("New")) {

            }
            if (ImGui::MenuItem("Open")) {
                scene_open = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window")) {
            GamePanel &gamePanel = GamePanel::GetInstance();
            if (ImGui::MenuItem("Game", nullptr, gamePanel.IsOpen())) {
                gamePanel.Toggle();
            }
            ScenePanel &scenePanel = ScenePanel::GetInstance();
            if (ImGui::MenuItem("Scene", nullptr, ScenePanel::GetInstance().IsOpen())) {
                scenePanel.Toggle();
            }
            ImGui::EndMenu();
        }

        ProjectDialog &projectDialog = ProjectDialog::GetInstance();
        if (project_new) {
            ImGui::OpenPopup("New Project");
        }
        projectDialog.CreateImGui();

        FileDialog &fileDialog = FileDialog::GetInstance();
        if (project_open) {
            fileDialog.SetFolderDialog(true);
            fileDialog.SetCallback([](const string &path) {
                // find the project file 
                string pfile;
                auto fspath = filesystem::u8path(path);
                for (auto &p : filesystem::directory_iterator(fspath)) {
                    // find .json file that has the same name with the selected directory
                    if (p.path().stem().u8string() == fspath.stem().u8string() &&
                            p.path().extension().string() == ".json") {
                        pfile = p.path().u8string();
                        break;
                    }
                }

                // load the Project
                Project &project = Project::GetInstance();
                project.Load(pfile);
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
