#include <filesystem>
#include <fstream>
#include <cstdint>

#include <nlohmann/json.hpp>

#include <Project.hpp>
#include <Graphics/Window.hpp>
#include <ProjectDialog.hpp>
#include <FileDialog.hpp>
#include <NewDialog.hpp>
#include <HierarchyPanel.hpp>
#include <MainMenuBar.hpp>

using json = nlohmann::json;
using namespace std;

const string &ProjectDialog::GetProjectsPath() {
    static std::string projectsPath(std::string(NEWBIE_PATH) + "/build/Editor/projects.json"); 
    return projectsPath; 
}

const string &ProjectDialog::GetEmptyProjectPath() {
    static std::string emptyProjectPath(std::string(NEWBIE_PATH) + "/Samples/Empty");
    return emptyProjectPath;
}

ProjectDialog::ProjectDialog() : Dialog("Open Project") {
    flags = ImGuiWindowFlags_MenuBar;
    width = 600.0f;

    try {
        ifstream fs(GetProjectsPath());
        if (fs.fail()) {
            throw exception();
        }
        json js;
        fs >> js;
        projects = js.get<std::unordered_set<std::string>>();
    } catch (...) {
        ofstream fs(GetProjectsPath());
        json js = std::unordered_set<std::string>();
        fs << js;
    }
}

void ProjectDialog::AddProject(const string &dir) {
    projects.insert(dir);
    ofstream fs(GetProjectsPath());
    json js(projects);
    fs << js;
}

void ProjectDialog::RemoveProject(const string &dir) {
    projects.erase(dir);
    ofstream fs(GetProjectsPath());
    json js(projects);
    fs << js;
}

void ProjectDialog::LoadProject(const string &dir) {
    // clear temporary copies
    HierarchyPanel::GetInstance().Clear();

    Project &project = Project::GetInstance();
    if (project.Load(GetProjectFile(dir))) {
        Window &window = Window::GetInstance();
        window.SetTitle(string("Newbie - ") + project.GetName());
    }
    GetLocalSelected() = nullptr;
    GetSelected() = nullptr; // nothing should be selected after the project loads
    MainMenuBar::GetInstance().StopGamePlay();
    ImGui::CloseCurrentPopup();
}

string ProjectDialog::GetProjectFile(const string &dir) const {
    string ret;
    auto path = filesystem::u8path(dir);
    for (auto &p : filesystem::directory_iterator(path)) {
        // find file that has the same name with the selected directory
        if (p.path().stem().u8string() == path.stem().u8string()) {
            ret = p.path().u8string();
            break;
        }
    }
    return ret;
}

void ProjectDialog::ShowContents() {
    FileDialog &fileDialog = FileDialog::GetInstance();
    if (ImGui::BeginMenuBar()) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg]);
        if (ImGui::Button("New")) {
            newProject = true;
        }
        if (ImGui::Button("Find...")) {
            fileDialog.SetFolderDialog();
            fileDialog.SetCallback([this](const string &path) {
                AddProject(path);
            });
            fileDialog.Open();
        }
        ImGui::PopStyleColor();
        fileDialog.Show();  

        ImGui::EndMenuBar();
    }
    
    ImGui::BeginChild("List", ImVec2(0.0f, 360.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    const string *deleted = nullptr;
    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ChildBg]);
    for (const string &path : projects) {
        if (ImGui::Button((string(ICON_FA_TIMES"##") + to_string((uintptr_t)&path)).c_str())) {
            deleted = &path;
        }
        ImGui::SameLine();
        if (ImGui::Selectable(path.c_str(), GetLocalSelected() == (void *)&path, ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::IsMouseDoubleClicked(0)) {
                LoadProject(path);
            } else {
                GetLocalSelected() = (void *)&path;
            }
        }
    }
    ImGui::PopStyleColor();
    if (deleted) {
        GetLocalSelected() = nullptr;
        RemoveProject(*deleted);
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::Indent(ImGui::GetWindowWidth() - 95.0f);
    if (ImGui::Button("Select", ImVec2(80.0f, 0.0f))) {
        if (GetLocalSelected()) {
            LoadProject(*(string *)GetLocalSelected());
        }
    }

    NewDialog &newDialog = NewDialog::GetInstance();
    if (newProject) {
        newDialog.SetCallback([this](const string &path)->bool {
            auto sample(filesystem::u8path(GetEmptyProjectPath()));
            auto created(filesystem::u8path(path));
            string name = created.stem().u8string();
            try {
                // copy files from "Samples/Empty" (non-recursive)
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

                AddProject(created.u8string());
            } catch (...) { return false; }
            return true;
        });
        newDialog.Open();
        newProject = false;
    }
    newDialog.Show();
}