#include <fstream>
#include <filesystem>

#include <AssetPanel.hpp>
#include <GamePanel.hpp>
#include <Engine.hpp>

using namespace std;
using json = nlohmann::json;
using namespace Engine;

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
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
        ImGui::Text(ICON_FA_CUBE);
        ImGui::PopStyleColor();
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
            if (ImGui::Selectable((asset->GetName() + "##" + to_string((uintptr_t)asset)).c_str(), (void *)asset == selected, ImGuiSelectableFlags_AllowDoubleClick)) {
                if (ImGui::IsMouseDoubleClicked(0)) {
                    if (asset->GetType() == APrefab::StaticType() && !GamePanel::GetInstance().IsGameRunning()) {
                        if (preview) {
                            Scene::FromBackup();
                        }
                        aprefab = (APrefab *)asset;
                        Scene &scene = Scene::GetInstance();
                        Scene::ToBackup();
                        scene.FromJson(aprefab->GetJson());
                        scene.SetLoaded(true);
                        preview = true;
                    }
                }
            }
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
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject")) {
            IM_ASSERT(payload->DataSize == sizeof(GameObject *));
            GameObject *gameObject = *(GameObject **)payload->Data;
            if (!gameObject->GetPrefab()) {
                APrefab *aprefab = project.AddAsset<APrefab>();
                aprefab->SetName(gameObject->GetName());
                aprefab->SetPrefab(gameObject);
                GameObject::ToJson(aprefab->GetJson(), vector<GameObject *>{gameObject}, true);
                aprefab->Apply();
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void AssetPanel::ShowPreviewOff() {
    ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
    if (ImGui::Selectable(ICON_FA_ANGLE_DOUBLE_LEFT, false, 0, ImVec2(16.0f, 0.0f))) {
        preview = false;
        Scene &scene = Scene::GetInstance();
        scene.ToJson(aprefab->GetJson());
        Scene::FromBackup();
        shared_ptr<Prefab> prefab = dynamic_pointer_cast<Prefab>(aprefab->GetResource());
        prefab->SetDirty(true);
        aprefab = nullptr;
    }
    ImGui::PopStyleColor();
}