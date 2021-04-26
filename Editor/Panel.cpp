#include <Panel.hpp>

void Panel::Show() {
    Update();
    if (!open) {
        return;
    }

    ImGui::SetNextWindowSizeConstraints(ImVec2(128, 128), ImVec2(FLT_MAX, FLT_MAX));
    if (!ImGui::Begin(title.c_str(), &open, flags)) {
        focus = false;
        ImGui::End();
        return;
    }
    if (!focus && ImGui::IsWindowFocused()) {
        selected = nullptr;
    }
    focus = ImGui::IsWindowFocused();
    ShowContents();
    ImGui::End();
}