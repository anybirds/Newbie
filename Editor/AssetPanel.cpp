#include <fstream>
#include <filesystem>
#include <functional>

#include <Engine.hpp>
#include <AssetPanel.hpp>
#include <GamePanel.hpp>
#include <NewDialog.hpp>

using namespace std;
using json = nlohmann::json;

void AssetPanel::ShowContents() {
    Project &project = Project::GetInstance();
    if (!project.IsLoaded()) {
        ImGui::End();
        return;
    }

    ImGui::BeginChild("Asset", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (auto &p : project.GetAllAssets()) {
        Asset *asset = p.second;
        ShowIcon(asset->GetType());
        ImGui::SameLine();
        if (IsRenaming() && !GetLocalSelected() && GetSelected() == (Entity *)asset) {
            string name = asset->GetName();
            ShowRenamingItem(name);
            asset->SetName(name);
        } else {
            ImGui::PushID(asset);
            if (ImGui::Selectable(asset->GetName().c_str(), GetSelected() == (void *)asset)) {
                GetSelected() = (Entity *)asset;
            }
            ImGui::PopID();
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                static Asset *source;
                source = asset;
                ImGui::SetDragDropPayload("Asset", &source, sizeof(Asset *));
                ShowIcon(source->GetType());
                ImGui::SameLine();
                ImGui::Text(source->GetName().c_str());
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