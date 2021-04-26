#include <fstream>
#include <filesystem>

#include <AssetPanel.hpp>
#include <NewDialog.hpp>
#include <Engine.hpp>

using namespace std;
using json = nlohmann::json;
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
        ImGui::SameLine();
        if (rename && selected == (void *)asset) {
            ShowRename(asset->GetName()); 
        } else {
            if (ImGui::Selectable((asset->GetName() + "##" + to_string((uint64_t)asset)).c_str(), (void *)asset == selected, ImGuiSelectableFlags_AllowDoubleClick)) {
                if (ImGui::IsMouseDoubleClicked(0)) {
                    if (type == APrefab::StaticType()) {
                        if (preview) {
                            Scene::FromBackup();
                        }
                        APrefab *aprefab = (APrefab *)asset;
                        Scene &scene = Scene::GetInstance();
                        scene.LoadImmediate(aprefab->GetPath(), true);
                        preview = true;
                    }
                }
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0)) {
                selected = (void *)asset;
            }
        }
    }
    ImGui::EndChild();
    NewDialog &newDialog = NewDialog::GetInstance();
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
        {
            IM_ASSERT(payload->DataSize == sizeof(GameObject *));
            gameObject = *(GameObject **)payload->Data;
            if (!gameObject->GetPrefab()) {
                newDialog.SetCallback([this, &project](const string &path)->bool {
                    try {
                        APrefab *aprefab = project.AddAsset<APrefab>();
                        aprefab->SetName(gameObject->GetName());
                        aprefab->SetPath(filesystem::relative(path, project.GetDirectoy()).u8string());
                        aprefab->SetPrefab(gameObject);

                        json js;
                        GameObject::ToJson(js, vector<GameObject *>{this->gameObject}, true);
                        ofstream fs(path);
                        if (fs.fail()) {
                            return false;
                        }
                        fs << js;
                    } catch (...) { return false; }
                    return true;
                });
                newDialog.Open();
            }
        }
        ImGui::EndDragDropTarget();
    }

    newDialog.Show();
}

void AssetPanel::ShowPreviewOff() {
    ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
    if (ImGui::Selectable(ICON_FA_ANGLE_DOUBLE_LEFT, false, 0, ImVec2(16.0f, 0.0f))) {
        preview = false;
        Scene::FromBackup();
    }
    ImGui::PopStyleColor();
}