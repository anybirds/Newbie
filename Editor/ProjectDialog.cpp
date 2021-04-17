#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include <Project.hpp>
#include <ProjectDialog.hpp>
#include <FileDialog.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

const string &ProjectDialog::GetProjectsFile() {
    static std::string projectsFile(std::string(NEWBIE_PATH) + "/projects.json"); 
    return projectsFile; 
}

const string &ProjectDialog::GetEmptyProject() {
    static std::string emptyProject(std::string(NEWBIE_PATH) + "/Samples/Empty");
    return emptyProject;
}

ProjectDialog::ProjectDialog() : Dialog("Open Project") {
    width = 600.0f;

    try {
        ifstream fs(GetProjectsFile());
        if (fs.fail()) {
            throw exception();
        }
        json js;
        fs >> js;
        projects = js.get<std::unordered_set<std::string>>();
    } catch (...) {
        ofstream fs(GetProjectsFile());
        json js = std::unordered_set<std::string>();
        fs << js;
    }
}

void ProjectDialog::AddProject(const string &dir) {
    projects.insert(dir);
    ofstream fs(GetProjectsFile());
    json js(projects);
    fs << js;
}

void ProjectDialog::RemoveProject(const string &dir) {
    projects.erase(dir);
    ofstream fs(GetProjectsFile());
    json js(projects);
    fs << js;
}

void ProjectDialog::LoadProject(const string &dir) {
    Project &project = Project::GetInstance();
    project.Load(GetProjectFile(dir));
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
    if (ImGui::Button("New")) {
        newProject = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Find...")) {
        fileDialog.SetFolderDialog();
        fileDialog.SetCallback([this](const string &path) {
            AddProject(path);
        });
        fileDialog.Open();
    }
    fileDialog.Show();
    ImGui::Separator();
    ImGui::BeginChild("List", ImVec2(0.0f, 360.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    const string *deleted = nullptr;
    for (const string &path : projects) {
        if (ImGui::Button((string(ICON_FA_TIMES"##") + to_string((uintptr_t)&path)).c_str())) {
            deleted = &path;
        }
        ImGui::SameLine();
        if (ImGui::Selectable(path.c_str(), selected == (void *)&path, ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::IsMouseDoubleClicked(0)) {
                selected = nullptr;
                LoadProject(path);
            } else {
                selected = (void *)&path;
            }
        }
    }
    if (deleted) {
        selected = nullptr;
        RemoveProject(*deleted);
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::Indent(ImGui::GetWindowWidth() - 95.0f);
    if (ImGui::Button("Select", ImVec2(80.0f, 0.0f))) {
        if (selected) {
            LoadProject(*(string *)selected);
            selected = nullptr;
        }
    }

    if (newProject) {
        ImGui::OpenPopup("New Project");
        selected = nullptr;
        newProject = false;
    }

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
                auto sample(filesystem::u8path(GetEmptyProject()));
                auto created(filesystem::u8path(location + "/" + name));
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

                    name.clear();
                    location.clear();
                    ImGui::CloseCurrentPopup();
                } catch (...) {}
            }
        }

        ImGui::EndPopup();
    }
}