#include <Dialog.hpp>

void Dialog::Show() {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(480.0f, 0.0f));

    bool p_open = true;
    if (ImGui::BeginPopupModal(title.c_str(), &p_open, ImGuiWindowFlags_NoResize)) {
        ShowContents();
        UpdateRename();
        ImGui::EndPopup();
    }
}