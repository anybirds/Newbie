#include <filesystem>
#include <vector>
#include <cstdint>

#include <FileDialog.hpp>

using namespace std;

void FileDialog::ShowContents() {
    string temp(dir);
    auto path(filesystem::absolute(filesystem::u8path(dir)));
    vector<pair<string, filesystem::path>> split;
    while (path.has_relative_path()) {
        split.push_back(make_pair(path.filename().u8string(), path));
        path = path.parent_path();
    }
    split.push_back(make_pair(path.root_name().u8string(), path.root_path()));

    uintptr_t index = 0;
    ImGui::BeginChild("File", ImVec2(0.0f, 35.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto it = split.rbegin(); it != split.rend(); it++) {
        ImGui::SameLine();
        if (it != split.rbegin()) {
            ImGui::Text(ICON_FA_ANGLE_RIGHT);
            ImGui::SameLine();
        }
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ChildBg]);
        if (ImGui::SmallButton((it->first + "##" + to_string(index)).c_str())) {
            temp = it->second.u8string();
        }
        ImGui::PopStyleColor();
        index++;
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::BeginChild("List", ImVec2(0.0f, 320.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    index = 1;
    for (auto &p : filesystem::directory_iterator(filesystem::u8path(dir))) {
        try {
            string name(p.path().filename().u8string());
            if (folder) {
                if (filesystem::is_directory(p.path())) {
                    ImGui::Text(ICON_FA_FOLDER);
                    ImGui::SameLine();
                    if (ImGui::Selectable((name + "##" + to_string(index)).c_str(), GetLocalSelected() == (void *)index, ImGuiSelectableFlags_AllowDoubleClick)) {
                        if (ImGui::IsMouseDoubleClicked(0)) {
                            temp = p.path().u8string();
                        }
                        GetLocalSelected() = (void *)index;
                        arg = p.path().u8string();
                    }
                }
            } else {
                if (filesystem::is_directory(p.path())) {
                    ImGui::Text(ICON_FA_FOLDER);
                } else {
                    ImGui::Text(ICON_FA_FILE);
                }
                ImGui::SameLine();
                if (ImGui::Selectable(name.c_str(), GetLocalSelected() == (void *)index, ImGuiSelectableFlags_AllowDoubleClick)) {
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        if (filesystem::is_directory(p.path())) {
                            temp = p.path().u8string();
                        } else {
                            callback(p.path().u8string());
                            GetLocalSelected() = nullptr;
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    GetLocalSelected() = (void *)index;
                    arg = p.path().u8string();
                }
            }
        } catch(...) {}
        index++;
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 95.0f);
    if (ImGui::Button("Select", ImVec2(80.0f, 0.0f))) {
        if (GetLocalSelected() && (folder || filesystem::is_regular_file(filesystem::u8path(arg)))) {
            callback(arg);
            GetLocalSelected() = nullptr;
            ImGui::CloseCurrentPopup();
        } else if (folder) {
            callback(dir);
            GetLocalSelected() = nullptr;
            ImGui::CloseCurrentPopup();
        }
    }

    if (dir != temp) {
        dir = temp;
        GetLocalSelected() = nullptr;
        arg.clear();
    }
}