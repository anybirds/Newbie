#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>

#include <AssetPanel.hpp>
#include <GamePanel.hpp>
#include <NewDialog.hpp>
#include <Engine.hpp>

using namespace std;
using json = nlohmann::json;

void AssetPanel::ShowIcon(Asset *asset) {
    Type *type = asset->GetType();
    if (type == AMaterial::StaticType()) {
        ImGui::Text(ICON_FA_TINT);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Material");
    } else if (type == AMesh::StaticType()) {
        ImGui::Text(ICON_FA_DRAW_POLYGON);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Mesh");
    } else if (type == ATexture::StaticType()) {
        ImGui::Text(ICON_FA_IMAGE);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Texture");
    } else if (type == AShader::StaticType()) {
        ImGui::Text(ICON_FA_FILE_CODE);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Shader");
    } else if (type == AModel::StaticType()) {
        ImGui::Text(ICON_FA_SHAPES);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Model");
    } else if (type == AFramebuffer::StaticType()) {
        ImGui::Text(ICON_FA_DESKTOP);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Framebuffer");
    } else if (type == APrefab::StaticType()) {
        ImGui::Text(ICON_FA_CUBE);
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Prefab");
    } else {
        ImGui::Text(ICON_FA_FILE);
    }
}

void AssetPanel::ShowContents() {
    Project &project = Project::GetInstance();
    if (!project.IsLoaded()) {
        ImGui::End();
        return;
    }

    ImGui::BeginChild("Asset", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto &p : project.GetAllAssets()) {
        Asset *asset = p.second;
        ShowIcon(asset);
        ImGui::SameLine();
        if (rename && selected == (void *)asset) {
            ShowRename(asset->GetName()); 
        } else {
            ImGui::Selectable((asset->GetName() + "##" + to_string((uintptr_t)asset)).c_str(), (void *)asset == selected);
            if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0)) {
                selected = (void *)asset;
            }
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                ImGui::SetDragDropPayload("Asset", &selected, sizeof(Asset *));
                ShowIcon((Asset *)selected);
                ImGui::SameLine();
                ImGui::Text(((Asset *)selected)->GetName().c_str());
                ImGui::EndDragDropSource();
            }
        }
    }
    ImGui::EndChild();
    
    NewDialog &newDialog = NewDialog::GetInstance();
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
            IM_ASSERT(payload->DataSize == sizeof(GameObject *));
            GameObject *gameObject = *(GameObject **)payload->Data;
            newDialog.SetCallback([gameObject](const string &path)->bool {
                auto fspath(filesystem::u8path(path));
                try {
                    // write to prefab file
                    json js;
                    GameObject::ToJson(js, gameObject);
                    ofstream fs(fspath);
                    if (fs.fail()) {
                        return false;
                    } 
                    fs << js;

                    // add prefab
                    Project &project = Project::GetInstance();
                    APrefab *aprefab = project.AddAsset<APrefab>();
                    aprefab->SetName(gameObject->GetName());
                    aprefab->SetPath(filesystem::relative(fspath, filesystem::u8path(project.GetDirectoy())).u8string());
                } catch (...) { return false; }
                return true;
            });
            newDialog.Open();
        }
        ImGui::EndDragDropTarget();
    }
    newDialog.Show();
}