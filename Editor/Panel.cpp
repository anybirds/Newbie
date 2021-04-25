#include <Panel.hpp>

void Panel::Show() {
    Update();
    if (!open) {
        return;
    }

    ImGui::SetNextWindowSizeConstraints(ImVec2(128, 128), ImVec2(FLT_MAX, FLT_MAX));
    if (!ImGui::Begin(title.c_str(), &open, flags)) {
        ImGui::End();
        return;
    }
    ShowContents();
    ImGui::End();
}