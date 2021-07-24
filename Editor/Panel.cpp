#include <Panel.hpp>
#include <Engine.hpp>

using namespace std;

void Panel::Show() {
    Update();
    if (!open) {
        return;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(1024.0f, 768.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(128, 128), ImVec2(FLT_MAX, FLT_MAX));
    if (!ImGui::Begin(title.c_str(), &open, flags)) {
        ImGui::End();
        return;
    }
    ShowContents();
    ImGui::End();
}

void Panel::ShowIcon(Type *type) {
    if (type == GameObject::StaticType()) {
        ImGui::Text(ICON_FA_CUBE);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("GameObject");
    } else if (type == AMaterial::StaticType()) {
        ImGui::Text(ICON_FA_ADJUST);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Material");
    } else if (type == AMesh::StaticType()) {
        ImGui::Text(ICON_FA_DRAW_POLYGON);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Mesh");
    } else if (type == ATexture::StaticType()) {
        ImGui::Text(ICON_FA_IMAGE);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Texture");
    } else if (type == AShader::StaticType()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
        ImGui::Text(ICON_FA_SCROLL);
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Shader");
    } else if (type == AModel::StaticType()) {
        ImGui::Text(ICON_FA_BOXES);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Model");
    } else if (type == AFramebuffer::StaticType()) {
        ImGui::Text(ICON_FA_LAYER_GROUP);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Framebuffer");
    } else if (type == APrefab::StaticType()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
        ImGui::Text(ICON_FA_CUBE);
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Prefab");
    } else if (type == Transform::StaticType()) {
        ImGui::Text(ICON_FA_PROJECT_DIAGRAM);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Transform");
    } else if (type == Camera::StaticType()) {
        ImGui::Text(ICON_FA_CAMERA);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Camera");
    } else if (type == Drawer::StaticType()) {
        ImGui::Text(ICON_FA_PAINT_BRUSH);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Drawer");
    } else {
        ImGui::Text(ICON_FA_SCROLL);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip(type->GetName().c_str());
    }
}