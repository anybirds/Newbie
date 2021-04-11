#include <imgui/imgui.h>

#include <Engine.hpp>
#include <GamePanel.hpp>

using namespace std;
using namespace Engine;

GamePanel::GamePanel() {
    Window &window = Window::GetInstance();

    gameTexture = new ATexture();
    gameTexture->SetWidth(window.GetMonitorWidth());
    gameTexture->SetHeight(window.GetMonitorHeight());

    gameFramebuffer = new AFramebuffer();
    gameFramebuffer->SetColorTexture(gameTexture);
    gameFramebuffer->SetUseDepthRenderTexture(true);

    gameFramebufferResource = dynamic_pointer_cast<Framebuffer>(gameFramebuffer->GetResource());
    
    Camera::SetDefaultFramebuffer(gameFramebufferResource);
}

void GamePanel::Close() {
    gameFramebufferResource.reset();
    delete gameFramebuffer;
    delete gameTexture;
}

void GamePanel::CreateImGui() {
    Scene &scene = Scene::GetInstance();
    if (scene.IsLoaded()) {
        scene.Render();
    }

    if (!open) {
        return;
    }

    ImGui::Begin("Game", &open);

    if (scene.IsLoaded()) {
        Project &project = Project::GetInstance();
        // todo: set game framebuffer width, height to reference width, height

        ImVec2 imgSize = ImGui::GetContentRegionAvail();

        // todo: do fancy uv stuff to preserve aspect ratio
        ImGui::Image((void *)(intptr_t)gameFramebufferResource->GetColorTexture()->GetId(), 
        imgSize,
        ImVec2(0.0f, 1.0f),
        ImVec2(1.0f, 0.0f));
    }

    ImGui::End();
}