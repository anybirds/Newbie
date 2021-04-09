#include <filesystem>
#include <vector>
#include <iostream>

#include <imgui/imgui.h>
#include <Icons/IconsFontAwesome4.h>

#include <FileDialog.hpp>

using namespace std;

void FileDialog::CreateImGui() {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(480.0f, 0.0f));

    bool p_open = true;
    string title;
    if (folder) {
        title = "Open Folder";
    } else {
        title = "Open File";
    }
    if (ImGui::BeginPopupModal(title.c_str(), &p_open, ImGuiWindowFlags_NoResize)) {
        string temp(dir);
        auto path(filesystem::absolute(filesystem::path(dir)));
        vector<pair<string, filesystem::path>> split;
        while (path.has_relative_path()) {
            split.push_back(make_pair(path.filename().string(), path));
            path = path.parent_path();
        }
        split.push_back(make_pair(path.root_name().string(), path.root_path()));

        ImGui::BeginChild("file", ImVec2(0.0f, 40.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
        for (auto it = split.rbegin(); it != split.rend(); it++) {
            ImGui::SameLine();
            if (it != split.rbegin()) {
                ImGui::Text(ICON_FA_ANGLE_RIGHT);
                ImGui::SameLine();
            }
            if (ImGui::SmallButton(it->first.c_str())) {
                temp = it->second.string();
            }
        }
        ImGui::EndChild();

        ImGui::Separator();
        ImGui::BeginChild("list", ImVec2(0.0f, 480.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
        int index = 0;
        for (auto &p : filesystem::directory_iterator(dir)) {
            try {
                string name(p.path().filename().string());
                if (folder) {
                    if (filesystem::is_directory(p.path())) {
                        ImGui::Text(ICON_FA_FOLDER);
                        ImGui::SameLine();
                        if (ImGui::Selectable(name.c_str(), selected == index, ImGuiSelectableFlags_AllowDoubleClick)) {
                            if (ImGui::IsMouseDoubleClicked(0)) {
                                temp = p.path().string();
                            }
                            selected = index;
                            arg = p.path().string();
                        }
                    }
                } else {
                    if (filesystem::is_directory(p.path())) {
                        ImGui::Text(ICON_FA_FOLDER);
                    } else {
                        ImGui::Text(ICON_FA_FILE);
                    }
                    ImGui::SameLine();
                    if (ImGui::Selectable(name.c_str(), selected == index, ImGuiSelectableFlags_AllowDoubleClick)) {
                        if (ImGui::IsMouseDoubleClicked(0)) {
                            if (filesystem::is_directory(p.path())) {
                                temp = p.path().string();
                            } else {
                                callback(p.path().string());
                                ImGui::CloseCurrentPopup();
                            }
                        }
                        selected = index;
                        arg = p.path().string();
                    }
                }
            } catch(...) {}
            index++;
        }
        ImGui::EndChild();

        ImGui::Separator();
        ImGui::Indent(ImGui::GetWindowWidth() - 95.0f);
        if (ImGui::Button("Select", ImVec2(80.0f, 0.0f))) {
            if (selected >= 0) {
                callback(arg);
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();

        if (dir != temp) {
            dir = temp;
            selected = -1;
            arg.clear();
        }
    }
}