#include <Engine.hpp>

#include <imgui/imgui.h>

#include <ScenePanel.hpp>

using namespace Engine;

void ScenePanel::CreateImGui() {
    ImGui::SetNextWindowSize(ImVec2(800.0f, 480.0f));

    ImGui::Begin("Scene");
    Scene &scene = Scene::GetInstance();
    if (scene.IsLoaded()) {
        Project &project = Project::GetInstance();
        Camera *cam = project.GetSceneCamera()->GetComponent<Camera>();
        float imgWidth = ImGui::GetWindowSize().x - 5.0f; 
        float imgHeight = ImGui::GetWindowSize().y - 5.0f;
        cam->GetFramebuffer()->SetWidth((int)imgWidth);
        cam->GetFramebuffer()->SetHeight((int)imgHeight);
        cam->Render();
        ImGui::Image((void *)(intptr_t)cam->GetFramebuffer()->GetColorTexture()->GetId(), 
            ImVec2(imgWidth, imgHeight),
            ImVec2(0.0f, 0.0f),
            ImVec2(imgWidth / cam->GetFramebuffer()->GetMaxWidth(), imgHeight / cam->GetFramebuffer()->GetMaxHeight()));
    }
    ImGui::End();
}