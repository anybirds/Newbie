#include <filesystem>

#include <Project.hpp>
#include <Scene.hpp>
#include <SceneDialog.hpp>
#include <FileDialog.hpp>

using namespace std;
using namespace Engine;

const string &SceneDialog::GetEmptyScene() {
    static std::string emptyScene(string(NEWBIE_PATH) + "/Samples/Empty/Main.json");
    return emptyScene;
}

void SceneDialog::ShowContents() {
    FileDialog &fileDialog = FileDialog::GetInstance();
    if (ImGui::Button("New")) {
        newScene = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Find...")) {
        fileDialog.SetFileDialog();
        fileDialog.SetCallback([](const string &path) {
            Project &project = Project::GetInstance();
            project.AddScene(filesystem::relative(path, filesystem::u8path(project.GetDirectoy())).u8string());
        });
        fileDialog.Open();
    }
    fileDialog.Show();
    ImGui::Separator();
    ImGui::BeginChild("List", ImVec2(0.0f, 360.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    Project &project = Project::GetInstance();
    const string *deleted = nullptr;
    for (const string &path : project.GetAllScenes()) {
        if (ImGui::Button((string(ICON_FA_TIMES"##") + to_string((uintptr_t)&path)).c_str())) {
            deleted = &path;
        }
        ImGui::SameLine();
        if (ImGui::Selectable(path.c_str(), selected == (void *)&path, ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::IsMouseDoubleClicked(0)) {
                Scene &scene = Scene::GetInstance();
                scene.Load(path);
                selected = nullptr;
                ImGui::CloseCurrentPopup();
            } else {
                selected = (void *)&path;
            }
        }
    }
    if (deleted) {
        project.RemoveScene(*deleted);
        selected = nullptr;
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::Indent(ImGui::GetWindowWidth() - 95.0f);
    if (ImGui::Button("Select", ImVec2(80.0f, 0.0f))) {
        if (selected) {
            Scene &scene = Scene::GetInstance();
            scene.Load(*(string *)selected);
            selected = nullptr;
            ImGui::CloseCurrentPopup();
        }
    }

    if (newScene) {
        ImGui::OpenPopup("New Scene");
        selected = nullptr;
        newScene = false;
    }

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
            fileDialog.SetFolderDialog();
            fileDialog.SetCallback([this](const string &path) {
                this->location = path;
            });
            fileDialog.Open();
        }
        fileDialog.Show();

        ImGui::Separator();
        ImGui::Indent(ImGui::GetWindowWidth() - 95.0f);
        if (ImGui::Button("Create", ImVec2(80.0f, 0.0f))) {
            if (!name.empty() && !location.empty()) {
                auto sample(filesystem::u8path(GetEmptyScene()));
                auto created(filesystem::u8path(location + "/" + name));
                try {
                    // copy file "Samples/Empty/Main.json"
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
}