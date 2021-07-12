#include <filesystem>

#include <Project.hpp>
#include <Scene.hpp>
#include <MainMenuBar.hpp>
#include <FileDialog.hpp>
#include <SceneDialog.hpp>
#include <ProjectDialog.hpp>
#include <GamePanel.hpp>
#include <ScenePanel.hpp>
#include <AssetPanel.hpp>
#include <HierarchyPanel.hpp>

using namespace std;

void MainMenuBar::Show() {
    if (ImGui::BeginMainMenuBar())
    {
        bool project_new = false;
        bool project_open = false;
        bool scene_new = false;
        bool scene_open = false;

        Project &project = Project::GetInstance();
        GamePanel &gamePanel = GamePanel::GetInstance();
        if (ImGui::BeginMenu("Project"))
        {
            if (ImGui::MenuItem("New")) {
                project_new = true;
            }
            if (ImGui::MenuItem("Open")) {
                project_open = true;
            }
            if (ImGui::MenuItem("Save", nullptr, nullptr, project.IsLoaded() && !running)) {
                project.Save();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene", project.IsLoaded())) {
            if (ImGui::MenuItem("New")) {
                scene_new = true;
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
            if (ImGui::MenuItem("Scene", nullptr, scenePanel.IsOpen())) {
                scenePanel.Toggle();
            }
            AssetPanel &assetPanel = AssetPanel::GetInstance();
            if (ImGui::MenuItem("Asset", nullptr, assetPanel.IsOpen())) {
                assetPanel.Toggle();
            }
            HierarchyPanel &hierarchyPanel = HierarchyPanel::GetInstance();
            if (ImGui::MenuItem("Hierarchy", nullptr, hierarchyPanel.IsOpen())) {
                hierarchyPanel.Toggle();
            } 
            ImGui::EndMenu();
        }
        gamePanel.ShowPlayPause();

        ProjectDialog &projectDialog = ProjectDialog::GetInstance();
        if (project_new) {
            projectDialog.SetNewProject();
            projectDialog.Open();
        }
        if (project_open) {
            projectDialog.Open();
        }
        projectDialog.Show();

        SceneDialog &sceneDialog = SceneDialog::GetInstance();
        if (scene_new) {
            sceneDialog.SetNewScene();
            sceneDialog.Open();
        }
        if (scene_open) {
            sceneDialog.Open();
        }
        sceneDialog.Show();

        ImGui::EndMainMenuBar();
    }
}
