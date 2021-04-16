#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <Icons/IconsFontAwesome5.h>

#include <AssetPanel.hpp>
#include <Project.hpp>
#include <Asset.hpp>

#include <GL/glew.h>
#include <glfw/glfw3.h>

using namespace std;
using namespace Engine;

void AssetPanel::CreateImGui() {
    if (!open) {
        return;
    }

    ImGui::Begin("Asset", &open);

    Project &project = Project::GetInstance();
    if (!project.IsLoaded()) {
        ImGui::End();
        return;
    }

    ImGui::BeginChild("Asset", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto &p : project.GetAllAssets()) {
        Asset *asset = p.second;
        const string &name = asset->GetType()->GetName();
        if (name == "AMaterial") {
            ImGui::Text(ICON_FA_TINT);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Material");
        } else if (name == "AMesh") {
            ImGui::Text(ICON_FA_DRAW_POLYGON);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Mesh");
        } else if (name == "ATexture") {
            ImGui::Text(ICON_FA_IMAGE);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Texture");
        } else if (name == "AShader") {
            ImGui::Text(ICON_FA_FILE_CODE);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Shader");
        } else if (name == "AModel") {
            ImGui::Text(ICON_FA_SHAPES);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Model");
        } else if (name == "AFramebuffer") {
            ImGui::Text(ICON_FA_DESKTOP);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Framebuffer");
        } else {
            ImGui::Text(ICON_FA_FILE);
        }
        ImGui::SameLine();
        if (renamedAsset == asset) {
            if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
                ImGui::SetKeyboardFocusHere();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
            ImGui::InputText((string("##") + to_string((uint64_t)asset)).c_str(), &rename, ImGuiInputTextFlags_AutoSelectAll);
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        } else {
            if (ImGui::Selectable((asset->GetName() + "##" + to_string((uint64_t)asset)).c_str(), asset == selectedAsset)) {
                selectedAsset = asset;
            }
        }
    }
    if (selectedAsset && ImGui::IsKeyPressed(GLFW_KEY_F2)) {
        renamedAsset = selectedAsset;
        rename = renamedAsset->GetName();
    }
    if (renamedAsset && (ImGui::IsKeyPressed(GLFW_KEY_ENTER) || ImGui::IsMouseClicked(0))) {
        renamedAsset->SetName(rename);
        renamedAsset = nullptr;
    }
    ImGui::EndChild();

    ImGui::End();
}