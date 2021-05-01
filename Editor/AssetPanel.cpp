#include <iostream>
#include <fstream>
#include <filesystem>
#include <functional>

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
                    if (asset->GetType() == APrefab::StaticType() && !running) {
                        if (preview) {
                            Scene::FromBackup();
                        }
                        APrefab *aprefab = (APrefab *)asset;
                        Scene &scene = Scene::GetInstance();
                        Scene::ToBackup();
                        scene.FromJson(aprefab->GetJson());
                        scene.SetLoaded(true);
                        preview = true;

                        cerr << '[' << __FUNCTION__ << ']' << " preview Prefab: " << aprefab->GetName() << '\n';
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
                gameObject->SetPrefab(dynamic_pointer_cast<Prefab>(aprefab->GetResource()));
                GameObject::ToJson(aprefab->GetJson(), vector<GameObject *>{gameObject}, true);
                aprefab->Apply();

                cerr << '[' << __FUNCTION__ << ']' << " created Prefab: " << aprefab->GetName() << '\n';
            }
        }
        ImGui::EndDragDropTarget();
    }
}

void AssetPanel::ShowPreviewOff() {
    ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
    ImGuiSelectableFlags flags = ImGuiSelectableFlags_None;
    Scene &scene = Scene::GetInstance();
    assert(scene.GetRootGameObjects().size() == 1);
    if (scene.GetRootGameObjects()[0]->HasPrefabCycle()) {
        flags = ImGuiSelectableFlags_Disabled;
    }
    if (ImGui::Selectable(ICON_FA_ANGLE_DOUBLE_LEFT, false, flags, ImVec2(16.0f, 0.0f))) {
        shared_ptr<Prefab> prefab = scene.GetRootGameObjects()[0]->GetPrefab();
        APrefab *aprefab = (APrefab *)prefab->GetAsset();
        scene.ToJson(aprefab->GetJson(), true, prefab, prefab->GetEntityCount() + 1);
        Scene::FromBackup();
        prefab->SetDirty(true);
        aprefab = nullptr;
        preview = false;
    }
    ImGui::PopStyleColor();
}