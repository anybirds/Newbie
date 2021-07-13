#include <Dialog.hpp>

void Dialog::Show() {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(width, 0.0f));

    bool p_open = true;
    if (ImGui::BeginPopupModal(title.c_str(), &p_open, flags | ImGuiWindowFlags_NoResize)) {
        ShowContents();
        ImGui::EndPopup();
    }
}

void Dialog::Open() {
    ImGui::OpenPopup(title.c_str());
    GetLocalSelected() = nullptr;
}