#include <Panel.hpp>

void Panel::Show() {
    Update();
    if (!open) {
        return;
    }

    if (!ImGui::Begin(title.c_str(), &open, flags)) {
        ImGui::End();
        return;
    }
    ShowContents();
    ImGui::End();
}