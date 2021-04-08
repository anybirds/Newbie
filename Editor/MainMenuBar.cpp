#include <Engine.hpp>

#include <filesystem>
#include <string>

#include <imgui/imgui.h>

#include <MainMenuBar.hpp>
#include <FileDialog.hpp>

using namespace std;
using namespace Engine;

void MainMenuBar::CreateImGui() {
    if (ImGui::BeginMainMenuBar())
    {
        bool menu_item_open = false;
        if (ImGui::BeginMenu("Project"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")) {

            }
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                menu_item_open = true;
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                
            }
            ImGui::EndMenu();
        }

        FileDialog &fileDialog = FileDialog::GetInstance();
        if (menu_item_open) {
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
                if (!Project::Load(pfile)) {
                    return;
                }
                Project &project = Project::GetInstance();

                // load the start Scene
                if (!Scene::Load(project.GetSetting()->GetStartSceneIndex())) {
                    return;
                }
            });
            ImGui::OpenPopup("Open Folder");
        }
        fileDialog.CreateImGui();

        ImGui::EndMainMenuBar();
    }
}
