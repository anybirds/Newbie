#include <Engine.hpp>

#include <imgui/imgui.h>

#include <GamePanel.hpp>

using namespace Engine;

void GamePanel::CreateImGui() {
    ImGui::SetNextWindowSize(ImVec2(800.0f, 480.0f));

    ImGui::Begin("Game");
    Scene &scene = Scene::GetInstance();
    if (scene.IsLoaded()) {
        Project &project = Project::GetInstance();

        // todo: set game framebuffer width, height to reference width, height

        scene.Render();
        float imgWidth = ImGui::GetWindowSize().x - 5.0f;
        float imgHeight = ImGui::GetWindowSize().y - 5.0f;

        // todo: do fancy uv stuff to preserve aspect ratio
        ImGui::Image((void *)(intptr_t)project.GetGameFramebuffer()->GetColorTexture()->GetId(), 
            ImVec2(imgWidth, imgHeight),
            ImVec2(0.0f, 1.0f),
            ImVec2(1.0f, 0.0f));
    }
    ImGui::End();
}