#include <Widget.hpp>

#include <GL/glew.h>
#include <glfw/glfw3.h>

using namespace std;

void *Widget::selected;
bool Widget::rename;
bool Widget::renameClicked;

bool Widget::running;
bool Widget::paused;

void Widget::ShowRename(string &str) {
    ImGui::SetKeyboardFocusHere();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
    ImGui::InputText((string("##") + to_string((uintptr_t)selected)).c_str(), &str, ImGuiInputTextFlags_AutoSelectAll);
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    renameClicked = ImGui::IsItemClicked();
}

void Widget::UpdateRename() {
    if (selected && ImGui::IsKeyPressed(GLFW_KEY_F2)) {
        rename = true;
    }
    if (rename && (ImGui::IsKeyPressed(GLFW_KEY_ENTER) || ImGui::IsMouseClicked(0) && !renameClicked)) {
        rename = false;
    }
}