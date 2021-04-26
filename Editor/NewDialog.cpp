#include <NewDialog.hpp>
#include <FileDialog.hpp>

using namespace std;

void NewDialog::ShowContents() {
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
            if (callback(location + "/" + name)) {
                name.clear();
                location.clear();
                ImGui::CloseCurrentPopup();
            }
        }
    }
}