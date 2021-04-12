#include <Project.hpp>

#include <filesystem>
#include <string>

#include <imgui/imgui.h>

#include <MainMenuBar.hpp>
#include <FileDialog.hpp>
#include <SceneDialog.hpp>
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

        FileDialog &fileDialog = FileDialog::GetInstance();
        if (project_new) {
            /*
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(480.0f, 0.0f));
            bool p_open = true;
            if (ImGui::BeginPopupModal("New Project", &p_open, ImGuiWindowFlags_NoResize)) {
                ImGui::Text("Name: ");
                ImGui::SameLine();
                ImGui::InputText("", name, 128);
                
                ImGui::Text("Location: ");
                ImGui::SameLine();
                ImGui::InputText("", location, 512);
                ImGui::SameLine();
                ImGui::Button("Browse...");
                
                ImGui::EndPopup();
            }
            */
        }
        if (project_open) {
            fileDialog.SetFolderDialog(true);
            fileDialog.SetCallback([](const string &path) {
                // find the project file 
                string pfile;
                for (auto &p : filesystem::directory_iterator(filesystem::u8path(path))) {
                    if (p.path().extension().string() == ".json") {
                        pfile = p.path().u8string();
                        break;
                    }
                }

                // load the Project
                Project &project = Project::GetInstance();
                if (!project.Load(pfile)) {
                    return;
                }
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
