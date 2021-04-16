#include <Panel.hpp>

void Panel::Show() {
    if (!open) {
        return;
    }

    ImGui::Begin(title.c_str(), &open);
    ShowContents();
    UpdateRename();
    ImGui::End();
}