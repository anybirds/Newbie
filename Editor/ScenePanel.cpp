#include <Engine.hpp>
#include <ScenePanel.hpp>

using namespace std;
using namespace glm;
using namespace Engine;

const std::string &ScenePanel::GetSelectVertexShader() {
    static std::string selectVertexShader(std::string(NEWBIE_PATH) + "/Editor/Shaders/select_vert.glsl"); 
    return selectVertexShader;
}

const std::string &ScenePanel::GetSelectFragmentShader() {
    static std::string selectFragmentShader(std::string(NEWBIE_PATH) + "/Editor/Shaders/select_frag.glsl");
    return selectFragmentShader;
}

ScenePanel::ScenePanel() : Panel("Scene"), moveSpeed(5.0f), rotateSpeed(0.125f) {
    Window &window = Window::GetInstance();

    sceneTexture = new ATexture();
    sceneTexture->SetWidth(window.GetMonitorWidth());
    sceneTexture->SetHeight(window.GetMonitorHeight());

    sceneFramebuffer = new AFramebuffer();
    sceneFramebuffer->SetColorTexture(sceneTexture);
    sceneFramebuffer->SetUseDepthRenderTexture(true);
    sceneFramebufferResource = dynamic_pointer_cast<Framebuffer>(sceneFramebuffer->GetResource());

    sceneCamera = new GameObject();
    Transform *t = sceneCamera->AddComponent<Transform>();
    t->SetLocalPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    camera = sceneCamera->AddComponent<Camera>();
    camera->SetFramebuffer(sceneFramebufferResource);

    selectTexture = new ATexture();
    selectTexture->SetWidth(window.GetMonitorWidth());
    selectTexture->SetHeight(window.GetMonitorHeight());
    
    selectFramebuffer = new AFramebuffer();
    selectFramebuffer->SetColorTexture(selectTexture);
    selectFramebuffer->SetUseDepthRenderTexture(true);
    selectFramebufferResource = dynamic_pointer_cast<Framebuffer>(selectFramebuffer->GetResource());
    
    selectVertexShader = new AShader();
    selectVertexShader->SetShaderType(Shader::VERTEX);
    selectVertexShader->SetPath(GetSelectVertexShader());

    selectFragmentShader = new AShader();
    selectFragmentShader->SetShaderType(Shader::FRAGMENT);
    selectFragmentShader->SetPath(GetSelectFragmentShader());

    selectMaterial = new AMaterial();
    selectMaterial->SetVertexShader(selectVertexShader);
    selectMaterial->SetFragmentShader(selectFragmentShader);
    selectMaterialResource = dynamic_pointer_cast<Material>(selectMaterial->GetResource());
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
    try {
        if (clicked) {
            /*
            selectFramebufferResource->SetWidth((int)imgSize.x);
            selectFramebufferResource->SetHeight((int)imgSize.y);
            camera->SetFramebuffer(selectFramebufferResource);
            selectFramebufferResource->ReadPixels(clickX, clickY);
            */
        }
        sceneFramebufferResource->SetWidth((int)imgSize.x);
        sceneFramebufferResource->SetHeight((int)imgSize.y);
        camera->SetFramebuffer(sceneFramebufferResource);
        camera->Render();
    } catch(...) {}

    ImGui::Image((void *)(intptr_t)camera->GetFramebuffer()->GetColorTexture()->GetId(), 
        imgSize,
        ImVec2(0.0f, imgSize.y / camera->GetFramebuffer()->GetMaxHeight()),
        ImVec2(imgSize.x / camera->GetFramebuffer()->GetMaxWidth(), 0.0f));
    
    Input &input = Input::GetInstance();
    if (ImGui::IsItemClicked(1)) {
        control = true;
        input.SetMouseCursorMode(Input::MOUSE_CURSOR_DISABLED);
    }
    if (control && !ImGui::IsMouseDown(1)) {
        control = false;
        input.SetMouseCursorMode(Input::MOUSE_CURSOR_NORMAL);
    }

    clicked = ImGui::IsItemClicked(0);
    if (clicked) {
        clickX = (int)(ImGui::GetMousePos().x - ImGui::GetItemRectMin().x);
        clickY = (int)(ImGui::GetMousePos().y - ImGui::GetItemRectMin().y);
    }
}