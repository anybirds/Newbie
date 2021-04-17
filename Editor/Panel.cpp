#include <Panel.hpp>

void Panel::Show() {
    if (!open) {
        return;
    }

    ImGui::Begin(title.c_str(), &open, flags);
    ShowContents();
    ImGui::End();
}