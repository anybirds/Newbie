#include <filesystem>
#include <cstdint>

#include <Project.hpp>
#include <Scene.hpp>
#include <Graphics/Window.hpp>
#include <SceneDialog.hpp>
#include <FileDialog.hpp>
#include <NewDialog.hpp>
#include <MainMenuBar.hpp>

using namespace std;

const string &SceneDialog::GetEmptyScenePath() {
    static std::string emptyScenePath(string(NEWBIE_PATH) + "/Samples/Empty/Main.json");
    return emptyScenePath;
}

void SceneDialog::LoadScene(const std::string &path) {
    Scene &scene = Scene::GetInstance();
    if (scene.LoadImmediate(path)) {
        Window &window = Window::GetInstance();
        window.SetTitle(window.GetTitle() + " - " + scene.GetName());
    }
    GetLocalSelected() = nullptr;
    GetSelected() = nullptr; // nothing should be selected after the scece loads
    MainMenuBar::GetInstance().StopGamePlay();
    ImGui::CloseCurrentPopup();
}

void SceneDialog::ShowContents() {
    FileDialog &fileDialog = FileDialog::GetInstance();
    if (ImGui::BeginMenuBar()) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg]);
        if (ImGui::Button("New")) {
            newScene = true;
        }
        if (ImGui::Button("Find...")) {
            fileDialog.SetFileDialog();
            fileDialog.SetCallback([](const string &path) {
                Project &project = Project::GetInstance();
                project.AddScene(filesystem::relative(path, filesystem::u8path(project.GetDirectoy())).u8string());
            });
            fileDialog.Open();
        }
        ImGui::PopStyleColor();
        fileDialog.Show();

        ImGui::EndMenuBar();
    }
    
    ImGui::BeginChild("List", ImVec2(0.0f, 360.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    Project &project = Project::GetInstance();
    const string *deleted = nullptr;
    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ChildBg]);
    for (const string &path : project.GetAllScenes()) {
        if (ImGui::Button((string(ICON_FA_TIMES"##") + to_string((uintptr_t)&path)).c_str())) {
            deleted = &path;
        }
        ImGui::SameLine();
        if (ImGui::Selectable(path.c_str(), GetLocalSelected() == (void *)&path, ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::IsMouseDoubleClicked(0)) {
                LoadScene(path);
            } else {
                GetLocalSelected() = (void *)&path;
            }
        }
    }
    ImGui::PopStyleColor();
    if (deleted) {
        project.RemoveScene(*deleted);
        GetLocalSelected() = nullptr;
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::Indent(ImGui::GetWindowWidth() - 95.0f);
    if (ImGui::Button("Select", ImVec2(80.0f, 0.0f))) {
        if (GetLocalSelected()) {
            LoadScene(*(string *)GetLocalSelected());
        }
    }

    NewDialog &newDialog = NewDialog::GetInstance();
    if (newScene) {
        newDialog.SetCallback([](const string &path)->bool {
            auto sample(filesystem::u8path(GetEmptyScenePath()));
            auto created(filesystem::u8path(path));
            try {
                // copy file "Samples/Empty/Main.json"
                filesystem::copy_file(sample, created);

                // add scene
                Project &project = Project::GetInstance();
                project.AddScene(filesystem::relative(created, filesystem::u8path(project.GetDirectoy())).u8string());
            } catch (...) { return false; }
            return true;
        });
        newDialog.Open();
        newScene = false;
    }
    newDialog.Show();
}