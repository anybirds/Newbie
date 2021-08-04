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
#include <InspectorPanel.hpp>

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
            if (ImGui::MenuItem("Save", nullptr, nullptr, project.IsLoaded() && !gamePlaying)) {
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
            InspectorPanel &inspectorPanel = InspectorPanel::GetInstance();
            if (ImGui::MenuItem("Inspector", nullptr, inspectorPanel.IsOpen())) {
                inspectorPanel.Toggle();
            } 
            ImGui::EndMenu();
        }
        ShowReload();
        ShowPlayPause();

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

void MainMenuBar::ShowReload() {
    Project &project = Project::GetInstance();
    Scene &scene = Scene::GetInstance();
    ImGuiSelectableFlags flags = ImGuiSelectableFlags_None;
    if (!project.IsLoaded()) {
        flags = ImGuiSelectableFlags_Disabled;
    }
    if (ImGui::Selectable(ICON_FA_SYNC_ALT, false, flags, ImVec2(16.0f, 0.0f))) {
        string projectPath = project.GetPath();
        string scenePath = scene.GetPath();
        bool sceneLoaded = scene.IsLoaded();
        project.Save();
        project.Load(projectPath);
        if (sceneLoaded) {
            scene.LoadImmediate(scenePath);
        }
        // clear temporary copies
        AssetPanel::GetInstance().Clear();
        HierarchyPanel::GetInstance().Clear();
        InspectorPanel::GetInstance().Clear();

        GetLocalSelected() = nullptr;
        GetSelected() = nullptr; // nothing should be selected after the project loads
        StopGamePlay();
    }
}

void MainMenuBar::ShowPlayPause() {
    Scene &scene = Scene::GetInstance();
    ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
    ImGuiSelectableFlags flags = ImGuiSelectableFlags_None;
    if (!scene.IsLoaded()) {
        flags = ImGuiSelectableFlags_Disabled;
    }
    GamePanel &gamePanel = GamePanel::GetInstance();
    if (ImGui::Selectable(ICON_FA_PLAY, gamePlaying, flags, ImVec2(16.0f, 0.0f))) {
        if (gamePlaying) {
            gamePanel.SetOpen(false);
            Scene::FromBackup();
        } else {
            gamePanel.SetOpen(true);
            Scene::ToBackup();
            Scene::LoadBackup();
        }
        gamePlaying ^= true;
        gamePaused = false;

        GetLocalSelected() = nullptr;
        GetSelected() = nullptr; // nothing should be selected after the play mode changes
    }
    if (ImGui::Selectable(ICON_FA_PAUSE, gamePaused, flags, ImVec2(16.0f, 0.0f))) {
        gamePaused ^= true;
    }
    ImGui::PopStyleColor();
}