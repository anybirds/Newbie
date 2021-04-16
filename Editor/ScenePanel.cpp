#include <Engine.hpp>
#include <ScenePanel.hpp>

using namespace std;
using namespace Engine;

ScenePanel::ScenePanel() : Panel("Scene") {
    Window &window = Window::GetInstance();

    sceneTexture = new ATexture();
    sceneTexture->SetWidth(window.GetMonitorWidth());
    sceneTexture->SetHeight(window.GetMonitorHeight());

    sceneFramebuffer = new AFramebuffer();
    sceneFramebuffer->SetColorTexture(sceneTexture);
    sceneFramebuffer->SetUseDepthRenderTexture(true);

    sceneCamera = new GameObject();
    Transform *t = sceneCamera->AddComponent<Transform>();
    t->SetLocalPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    cam = sceneCamera->AddComponent<Camera>();
    cam->SetFramebuffer(dynamic_pointer_cast<Framebuffer>(sceneFramebuffer->GetResource()));
}

void ScenePanel::Close() {
    delete sceneCamera;
    delete sceneFramebuffer;
    delete sceneTexture;
}

void ScenePanel::ShowContents() {
    Scene &scene = Scene::GetInstance();
    if (!scene.IsLoaded()) {
        ImGui::End();
        return;
    }

    Project &project = Project::GetInstance();
    ImVec2 imgSize = ImGui::GetContentRegionAvail();
    cam->GetFramebuffer()->SetWidth((int)imgSize.x);
    cam->GetFramebuffer()->SetHeight((int)imgSize.y);
    try {
        cam->Render();
    } catch(...) {}
    ImGui::Image((void *)(intptr_t)cam->GetFramebuffer()->GetColorTexture()->GetId(), 
        imgSize,
        ImVec2(0.0f, imgSize.y / cam->GetFramebuffer()->GetMaxHeight()),
        ImVec2(imgSize.x / cam->GetFramebuffer()->GetMaxWidth(), 0.0f));
}