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
    static string search;
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
    
    static bool menu;
    if (ImGui::BeginPopupContextWindow())
    {
        if (!menu) {
            GetSelected() = (Entity *)hovered;
        }
        Asset *asset = (Asset *)GetSelected();
        if (ImGui::MenuItem("Copy", nullptr, nullptr, (bool)asset)) {
            GetCopyedType() = asset->GetType();
            GetCopyedType()->Serialize(GetCopyed(), asset);
        }
        if (ImGui::MenuItem("Paste", nullptr, nullptr, Type::IsBaseOf(Asset::StaticType(), GetCopyedType()) && !GetCopyed().empty())) {
            GetSelected() = project.AddAsset(GetCopyedType(), GetCopyed());
        }
        ImGui::Separator();
        if (ImGui::BeginMenu("Add Asset")) {
            // prefix-search asset types
            static string search;
            ImGui::Text(ICON_FA_SEARCH);
            ImGui::SameLine();
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::InputText("##Search", &search);
            ImGui::PopItemWidth();
            ImGui::Separator();

            htrie_map<char, Type *> trie;
            for (auto &p : Type::GetAllTypes()) {
                Type *type = p.second;
                if (!type->IsAbstract() && Type::IsBaseOf(Asset::StaticType(), type)) {
                    trie.insert(type->GetName(), type);   
                }
            }
            auto range = trie.equal_prefix_range(search);
            
            for (auto it = range.first; it != range.second; it++) {
                Type *type = *it;
                if (ImGui::MenuItem((GetIconCharacter(type) + type->GetName()).c_str())) {
                    GetSelected() = project.AddAsset(type);
                }
            }
            if (range.first == range.second) {
                ImGui::MenuItem("Nothing Found", nullptr, nullptr, false);
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

    Asset *asset;
    if (ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
        if (ImGui::IsKeyPressed(GLFW_KEY_C)) {
            if (GetSelected() && Type::IsBaseOf(Asset::StaticType(), GetSelected()->GetType())) {
                asset = (Asset *)GetSelected();
                GetCopyedType() = asset->GetType();
                GetCopyedType()->Serialize(GetCopyed(), asset);
            }
        } else if (ImGui::IsKeyPressed(GLFW_KEY_V)) {
            if (Type::IsBaseOf(Asset::StaticType(), GetCopyedType())) {
                GetSelected() = project.AddAsset(GetCopyedType(), GetCopyed());
            }
        }
    }
    if (ImGui::IsKeyPressed(GLFW_KEY_DELETE)) {
        if (GetSelected() && Type::IsBaseOf(Asset::StaticType(), GetSelected()->GetType())) {
            asset = (Asset *)GetSelected();
            asset->Remove();
            GetSelected() = nullptr;
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