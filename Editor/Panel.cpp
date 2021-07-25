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

const char *Panel::GetIconCharacter(Type *type) { 
    static unordered_map<Type *, const char *> iconMap {
        { Entity::StaticType(), ICON_FA_SQUARE },
        { Component::StaticType(), ICON_FA_BOX },
        { Asset::StaticType(), ICON_FA_BOX_OPEN },
        { Resource::StaticType(), ICON_FA_BOX_OPEN },
        { GameObject::StaticType(), ICON_FA_CUBE },

        { AMaterial::StaticType(), ICON_FA_ADJUST },
        { Material::StaticType(), ICON_FA_ADJUST },
        { AMesh::StaticType(), ICON_FA_DRAW_POLYGON },
        { Mesh::StaticType(), ICON_FA_DRAW_POLYGON },
        { ATexture::StaticType(), ICON_FA_IMAGE },
        { Texture::StaticType(), ICON_FA_IMAGE },
        { AShader::StaticType(), ICON_FA_MAGIC },
        { Shader::StaticType(), ICON_FA_MAGIC },
        { AModel::StaticType(), ICON_FA_BOXES },
        { Model::StaticType(), ICON_FA_BOXES },
        { AFramebuffer::StaticType(), ICON_FA_LAYER_GROUP },
        { Framebuffer::StaticType(), ICON_FA_LAYER_GROUP },
        { APrefab::StaticType(), ICON_FA_CUBES },
        { Prefab::StaticType(), ICON_FA_CUBES },
        
        { Transform::StaticType(), ICON_FA_PROJECT_DIAGRAM },
        { Renderer::StaticType(), ICON_FA_CAMERA },
        { Drawer::StaticType(), ICON_FA_PAINT_BRUSH },
        { Script::StaticType(), ICON_FA_SCROLL }
    };

    while (type) {
        auto it = iconMap.find(type);
        if (it != iconMap.end()) {
            return it->second;
        }
        type = type->GetBase();
    }

    // not reached 
    return nullptr;
}
void Panel::ShowIcon(Type *type) {
    const char *icon = GetIconCharacter(type);
    ImGui::Text(icon);
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(type->GetName().c_str());
}