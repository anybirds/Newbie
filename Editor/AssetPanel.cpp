#include <AssetPanel.hpp>
#include <Project.hpp>
#include <Asset.hpp>

using namespace std;
using namespace Engine;

void AssetPanel::ShowContents() {
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
        if (rename && selected == (void *)asset) {
            ShowRename(asset->GetName()); 
        } else {
            if (ImGui::Selectable((asset->GetName() + "##" + to_string((uint64_t)asset)).c_str(), (void *)asset == selected)) {
                selected = (void *)asset;
            }
        }
    }
    ImGui::EndChild();
}