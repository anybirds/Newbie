#include <Engine.hpp>
#include <ScenePanel.hpp>

using namespace std;
using namespace glm;
using namespace Engine;

ScenePanel::ScenePanel() : Panel("Scene"), moveSpeed(5.0f), rotateSpeed(0.125f) {
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

void ScenePanel::Update() {
    if (control) {
        Transform *transform = sceneCamera->GetTransform();
        Time &time = Time::GetInstance();
        float move = moveSpeed * time.GetDeltaTime();
        if (ImGui::IsKeyDown(GLFW_KEY_A)) {
            transform->Translate(vec3(-move, 0.0f, 0.0f));
        }
        if (ImGui::IsKeyDown(GLFW_KEY_D)) {
            transform->Translate(vec3(move, 0.0f, 0.0f));
        }
        if (ImGui::IsKeyDown(GLFW_KEY_S)) {
            transform->Translate(vec3(0.0f, 0.0f, move));
        }
        if (ImGui::IsKeyDown(GLFW_KEY_W)) {
            transform->Translate(vec3(0.0f, 0.0f, -move));
        }
        if (ImGui::IsKeyDown(GLFW_KEY_Q)) {
            transform->Translate(vec3(0.0f, -move, 0.0f));
        }
        if (ImGui::IsKeyDown(GLFW_KEY_E)) {
            transform->Translate(vec3(0.0f, move, 0.0f));
        }

        Input &input = Input::GetInstance();
        transform->Rotate(-rotateSpeed * vec3(input.GetDeltaMouseCursorY(), input.GetDeltaMouseCursorX(), 0.0f));
    }
}

void ScenePanel::ShowContents() {
    Scene &scene = Scene::GetInstance();
    if (!scene.IsLoaded()) {
        ImGui::End();
        return;
    }

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
    
    Input &input = Input::GetInstance();
    if (ImGui::IsItemClicked(1)) {
        control = true;
        input.SetMouseCursorMode(Input::MOUSE_CURSOR_DISABLED);
    }
    if (control && !ImGui::IsMouseDown(1)) {
        control = false;
        input.SetMouseCursorMode(Input::MOUSE_CURSOR_NORMAL);
    }
}