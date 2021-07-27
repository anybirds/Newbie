#include <fstream>
#include <filesystem>
#include <functional>
#include <vector>
#include <tsl/htrie_map.h>

#include <Engine.hpp>
#include <AssetPanel.hpp>
#include <GamePanel.hpp>
#include <NewDialog.hpp>

using namespace std;
using namespace tsl;
using json = nlohmann::json;

void AssetPanel::ShowContents() {
    Project &project = Project::GetInstance();
    if (!project.IsLoaded()) {
        ImGui::End();
        return;
    }

    ImGui::BeginChild("Asset", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);

    // prefix-search assets
    ImGui::Text(ICON_FA_SEARCH);
    ImGui::SameLine();
    ImGui::PushItemWidth(-FLT_MIN);
    ImGui::InputText("##Search", &search);
    ImGui::PopItemWidth();
    ImGui::Separator();

    Asset *hovered = nullptr;
    if (search.empty()) {
        for (auto &p : project.GetAllAssets()) {
            Asset *asset = p.second;
            if (IsRenaming() && !GetLocalSelected() && GetSelected() == (Entity *)asset) {
                ShowIcon(asset->GetType());
                ImGui::SameLine();
                string name = asset->GetName();
                ShowRenamingItem(name);
                asset->SetName(name);
            } else {
                ShowAsset(asset);
                if (ImGui::IsItemHovered()) {
                    hovered = asset;
                }
            }
        }
    } else {
        htrie_map<char, vector<Asset *>> trie;
        for (auto &p : project.GetAllAssets()) {
            auto it = trie.find(p.second->GetName());
            if (it == trie.end()) {
                trie.insert(p.second->GetName(), vector<Asset *>{p.second});
            } else {
                it->push_back(p.second);
            }
        }
        auto range = trie.equal_prefix_range(search);
        for (auto it = range.first; it != range.second; it++) {
            for (Asset *asset : *it) {
                ShowAsset(asset);
                if (ImGui::IsItemHovered()) {
                    hovered = asset;
                }
            }
        }
    }
    
    if (ImGui::BeginPopupContextWindow())
    {
        if (!menu) {
            GetSelected() = (Entity *)hovered;
        }
        Asset *asset = (Asset *)GetSelected();
        if (ImGui::MenuItem("Copy", nullptr, nullptr, (bool)asset)) {
            copyedType = asset->GetType();
            copyedType->Serialize(copyed, asset);
        }
        if (ImGui::MenuItem("Paste", nullptr, nullptr, !copyed.empty())) {
            project.AddAsset(copyedType, copyed);
        }
        ImGui::Separator();
        if (ImGui::BeginMenu("Add Asset")) {
            // prefix-search asset types
            ImGui::Text(ICON_FA_SEARCH);
            ImGui::SameLine();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::InputText("##Search", &searchAdd);
            ImGui::PopItemWidth();
            ImGui::Separator();

            htrie_map<char, Type *> trie;
            for (auto &p : Type::GetAllTypes()) {
                Type *type = p.second;
                if (!type->IsAbstract() && Type::IsBaseOf(Asset::StaticType(), type)) {
                    trie.insert(type->GetName(), type);   
                }
            }
            auto range = trie.equal_prefix_range(searchAdd);
            
            for (auto it = range.first; it != range.second; it++) {
                Type *type = *it;
                if (ImGui::MenuItem((GetIconCharacter(type) + type->GetName()).c_str())) {
                    project.AddAsset(type);
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Remove", nullptr, nullptr, (bool)asset)) {
            asset->Remove();
            GetSelected() = nullptr;
        }
        
        menu = true;
        ImGui::EndPopup();
    } else {
        menu = false;
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

void AssetPanel::ShowAsset(Asset *asset) {
    ShowIcon(asset->GetType());
    ImGui::SameLine();
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

void AssetPanel::Clear() {
    copyedType = nullptr;
    copyed.clear();
}