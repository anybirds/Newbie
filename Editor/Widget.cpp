#include <Widget.hpp>

#include <GL/glew.h>
#include <glfw/glfw3.h>

using namespace std;

void Widget::ShowRenamingItem(string &str) {
    ImGui::SetKeyboardFocusHere();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
    ImGui::InputText("##Rename", &str, ImGuiInputTextFlags_AutoSelectAll);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    IsClickedRenamingItem() = ImGui::IsItemClicked();
}

void Widget::Update() {
    if (IsSelected() && ImGui::IsKeyPressed(GLFW_KEY_F2)) {
        IsRenaming() = true;
    }
    if (IsRenaming() && (ImGui::IsKeyPressed(GLFW_KEY_ENTER) || ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1) && !IsClickedRenamingItem())) {
        IsRenaming() = false;
    }
}