#include <glm/gtc/type_ptr.hpp>

#include <Engine.hpp>
#include <ScenePanel.hpp>

using namespace std;
using namespace glm;

const std::string &ScenePanel::GetWhiteVertexShader() {
    static std::string whiteVertexShader(std::string(NEWBIE_PATH) + "/Editor/Shaders/white_vert.glsl"); 
    return whiteVertexShader;
}

const std::string &ScenePanel::GetWhiteFragmentShader() {
    static std::string whiteFragmentShader(std::string(NEWBIE_PATH) + "/Editor/Shaders/white_frag.glsl");
    return whiteFragmentShader;
}

const std::string &ScenePanel::GetOutlineVertexShader() {
    static std::string outlineVertexShader(std::string(NEWBIE_PATH) + "/Editor/Shaders/outline_vert.glsl"); 
    return outlineVertexShader;
}

const std::string &ScenePanel::GetOutlineFragmentShader() {
    static std::string outlineFragmentShader(std::string(NEWBIE_PATH) + "/Editor/Shaders/outline_frag.glsl");
    return outlineFragmentShader;
}

const std::string &ScenePanel::GetSelectVertexShader() {
    static std::string selectVertexShader(std::string(NEWBIE_PATH) + "/Editor/Shaders/select_vert.glsl"); 
    return selectVertexShader;
}

const std::string &ScenePanel::GetSelectFragmentShader() {
    static std::string selectFragmentShader(std::string(NEWBIE_PATH) + "/Editor/Shaders/select_frag.glsl");
    return selectFragmentShader;
}

ScenePanel::ScenePanel() : Panel("Scene"), 
    control(false), moveSpeed(10.0f), rotateSpeed(1.0f), 
    gizmoOperation(ImGuizmo::TRANSLATE), gizmoMode(ImGuizmo::WORLD) {
    Window &window = Window::GetInstance();

    sceneTexture = new ATexture();
    sceneTexture->SetWidth(window.GetMonitorWidth());
    sceneTexture->SetHeight(window.GetMonitorHeight());

    sceneFramebuffer = new AFramebuffer();
    sceneFramebuffer->SetColorTexture(sceneTexture);
    sceneFramebuffer->SetUseDepthRenderTexture(true);
    sceneFramebufferResource = dynamic_pointer_cast<Framebuffer>(sceneFramebuffer->GetResource());

    GameObject *sceneGameObject = new GameObject();
    sceneTransform = sceneGameObject->AddComponent<Transform>();
    sceneTransform->SetLocalPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    sceneCamera = sceneGameObject->AddComponent<Camera>();
    sceneCamera->SetFramebuffer(sceneFramebufferResource);

    whiteTexture = new ATexture();
    whiteTexture->SetWidth(window.GetMonitorWidth());
    whiteTexture->SetHeight(window.GetMonitorHeight());
    
    whiteFramebuffer = new AFramebuffer();
    whiteFramebuffer->SetColorTexture(whiteTexture);
    whiteFramebuffer->SetUseDepthRenderTexture(true);
    whiteFramebufferResource = dynamic_pointer_cast<Framebuffer>(whiteFramebuffer->GetResource());
    
    whiteVertexShader = new AShader();
    whiteVertexShader->SetShaderType(Shader::VERTEX);
    whiteVertexShader->SetPath(GetWhiteVertexShader());

    whiteFragmentShader = new AShader();
    whiteFragmentShader->SetShaderType(Shader::FRAGMENT);
    whiteFragmentShader->SetPath(GetWhiteFragmentShader());

    whiteMaterial = new AMaterial();
    whiteMaterial->SetVertexShader(whiteVertexShader);
    whiteMaterial->SetFragmentShader(whiteFragmentShader);
    whiteMaterialResource = dynamic_pointer_cast<Material>(whiteMaterial->GetResource());

    copyTexture = new ATexture();
    copyTexture->SetWidth(window.GetMonitorWidth());
    copyTexture->SetHeight(window.GetMonitorHeight());
    
    copyFramebuffer = new AFramebuffer();
    copyFramebuffer->SetColorTexture(copyTexture);
    copyFramebuffer->SetUseDepthRenderTexture(true);
    copyFramebufferResource = dynamic_pointer_cast<Framebuffer>(copyFramebuffer->GetResource());

    outlineVertexShader = new AShader();
    outlineVertexShader->SetShaderType(Shader::VERTEX);
    outlineVertexShader->SetPath(GetOutlineVertexShader());

    outlineFragmentShader = new AShader();
    outlineFragmentShader->SetShaderType(Shader::FRAGMENT);
    outlineFragmentShader->SetPath(GetOutlineFragmentShader());

    outlineMaterial = new AMaterial();
    outlineMaterial->SetVertexShader(outlineVertexShader);
    outlineMaterial->SetFragmentShader(outlineFragmentShader);
    outlineMaterialResource = dynamic_pointer_cast<Material>(outlineMaterial->GetResource());

    GameObject *outlineGameObject = new GameObject();
    outlineTransform = outlineGameObject->AddComponent<Transform>();
    outlineCamera = outlineGameObject->AddComponent<Camera>();
    outlineCamera->SetOrthographic(true);
    outlineCamera->SetAspectRatioFixed(true);
    outlineCamera->SetAspectRatio(1.0f);
    outlineCamera->SetNear(-1.0f);
    outlineCamera->SetFar(1.0f);
    outlineCamera->SetSize(1.0f);
    outlineDrawer = outlineGameObject->AddComponent<Drawer>();
    outlineDrawer->SetMaterial<shared_ptr<Material>>(outlineMaterialResource);
    outlineDrawer->SetMesh<shared_ptr<Mesh>>(Geometry::GetSquareMesh());

    selectTexture = new ATexture();
    selectTexture->SetWidth(window.GetMonitorWidth());
    selectTexture->SetHeight(window.GetMonitorHeight());
    selectTexture->SetInternalFormat(Texture::R32I);
    selectTexture->SetFormat(Texture::RED_INTEGER);
    selectTexture->SetDataType(Texture::INT);
    
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
    delete sceneTransform;
    delete sceneCamera;
    delete sceneTexture;
    delete sceneFramebuffer;
    sceneFramebufferResource.reset();

    delete whiteTexture;
    delete whiteFramebuffer;
    delete whiteVertexShader;
    delete whiteFragmentShader;
    delete whiteMaterial;
    whiteMaterialResource.reset();
    whiteFramebufferResource.reset();

    delete copyTexture;
    delete copyFramebuffer;
    copyFramebufferResource.reset();

    delete outlineTransform;
    delete outlineCamera;
    delete outlineDrawer;
    delete outlineVertexShader;
    delete outlineFragmentShader;
    delete outlineMaterial;
    outlineMaterialResource.reset();

    delete selectTexture;
    delete selectFramebuffer;
    delete selectVertexShader;
    delete selectFragmentShader;
    delete selectMaterial;
    selectMaterialResource.reset();
    selectFramebufferResource.reset();
}

void ScenePanel::Update() {
    if (control) {
        Transform *transform = sceneTransform;
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

        Window &window = Window::GetInstance();
        Input &input = Input::GetInstance();
        transform->Rotate(360.0f * -rotateSpeed * vec3(input.GetDeltaMouseCursorY() / window.GetWidth(), input.GetDeltaMouseCursorX() / window.GetHeight(), 0.0f));
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
        sceneFramebufferResource->SetWidth((int)imgSize.x);
        sceneFramebufferResource->SetHeight((int)imgSize.y);

        sceneCamera->Render();

        // render selected object outline
        if (GetSelected() && GetSelected()->GetType() == GameObject::StaticType()) {
            GameObject *gameObject = (GameObject *)GetSelected();
            vector<Drawer *> drawers = gameObject->GetComponents<Drawer>();
            if (!drawers.empty()) {
                // draw selected object with a single color
                whiteFramebufferResource->SetWidth((int)imgSize.x);
                whiteFramebufferResource->SetHeight((int)imgSize.y);
                sceneCamera->SetFramebuffer(whiteFramebufferResource);
                sceneCamera->Render([this, &drawers](){
                    for (Drawer *drawer : drawers) {
                        drawer->Draw(this->sceneCamera, this->whiteMaterialResource);
                    }
                });
                
                // copy scene framebuffer
                copyFramebufferResource->SetWidth((int)imgSize.x);
                copyFramebufferResource->SetHeight((int)imgSize.y);
                Framebuffer::Blit(sceneFramebufferResource, copyFramebufferResource);
                
                // postprocessing
                outlineMaterialResource->SetVec2("uvScale", 
                    vec2(imgSize.x / (float)sceneFramebufferResource->GetMaxWidth(), imgSize.y / (float)sceneFramebufferResource->GetMaxHeight()));
                outlineMaterialResource->SetSampler("sceneTexture", copyFramebufferResource->GetColorTexture());
                outlineMaterialResource->SetSampler("whiteTexture", whiteFramebufferResource->GetColorTexture());
                outlineCamera->SetFramebuffer(sceneFramebufferResource);
                outlineCamera->Render([this](){
                    this->outlineDrawer->Draw(this->outlineCamera);
                });
                sceneCamera->SetFramebuffer(sceneFramebufferResource);
            }
        }
    } catch(...) {}
  
    ImGui::Image((void *)(intptr_t)sceneCamera->GetFramebuffer()->GetColorTexture()->GetId(), 
        imgSize,
        ImVec2(0.0f, imgSize.y / sceneCamera->GetFramebuffer()->GetMaxHeight()),
        ImVec2(imgSize.x / sceneCamera->GetFramebuffer()->GetMaxWidth(), 0.0f));
    
    // viewport control
    Input &input = Input::GetInstance();
    if (ImGui::IsItemClicked(1)) {
        control = true;
        ImGui::SetWindowFocus();
        input.SetMouseCursorMode(Input::MOUSE_CURSOR_DISABLED);
    }
    if (control && !ImGui::IsMouseDown(1)) {
        control = false;
        input.SetMouseCursorMode(Input::MOUSE_CURSOR_NORMAL);
    }

    // gizmo control
    if (!ImGui::IsMouseDown(0) && !ImGui::IsMouseDown(1)) {
        if (ImGui::IsKeyPressed(GLFW_KEY_T)) {
            gizmoOperation = ImGuizmo::TRANSLATE;
        } else if (ImGui::IsKeyPressed(GLFW_KEY_R)) {
            gizmoOperation = ImGuizmo::ROTATE;
        } else if (ImGui::IsKeyPressed(GLFW_KEY_S)) {
            gizmoOperation = ImGuizmo::SCALE;
        }
        
        if (ImGui::IsKeyPressed(GLFW_KEY_G)) {
            gizmoMode = ImGuizmo::WORLD;
        } else if (ImGui::IsKeyPressed(GLFW_KEY_L)) {
            gizmoMode = ImGuizmo::LOCAL;
        }
    }
    
    // draw gizmo
    if (GetSelected() && GetSelected()->GetType() == GameObject::StaticType()) {
        ImGuizmo::SetDrawlist();
        GameObject *gameObject = (GameObject *)GetSelected();
        Transform *transform = gameObject->GetTransform();
        mat4 view = sceneTransform->GetWorldToLocalMatrix();
        mat4 normalization = sceneCamera->GetNormalization();
        mat4 object = transform->GetLocalToWorldMatrix();
        ImGuizmo::SetRect(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y, ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y);
        ImGuizmo::Manipulate(value_ptr(view), value_ptr(normalization), gizmoOperation, gizmoOperation == ImGuizmo::SCALE ? ImGuizmo::LOCAL : gizmoMode, value_ptr(object));
        if (ImGuizmo::IsUsing()) {
            vec3 translation, rotation, scale;
            mat4 local = transform->GetParent() ? transform->GetParent()->GetWorldToLocalMatrix() * object : object;
            ImGuizmo::DecomposeMatrixToComponents(value_ptr(local), value_ptr(translation), value_ptr(rotation), value_ptr(scale));
            transform->SetLocalPosition(translation);
            transform->SetLocalEulerAngles(rotation);
            transform->SetLocalScale(scale);
        }
    }

    // mouse picking
    if (!ImGuizmo::IsOver() && ImGui::IsItemClicked(0)) {
        try {
            selectFramebufferResource->SetWidth((int)imgSize.x);
            selectFramebufferResource->SetHeight((int)imgSize.y);
            sceneCamera->SetFramebuffer(selectFramebufferResource);
            vector<Drawer *> drawers;
            sceneCamera->Render([this, &scene, &drawers](){
                int base = 1;
                for (auto &order : scene.GetAllBatches()) {
                    for (auto &batch : order.second) {
                        for (Drawer *drawer : batch.second->GetDrawers()) {
                            drawers.push_back(drawer);
                        }
                        this->selectMaterialResource->SetInt("base", base);
                        batch.second->Draw(this->sceneCamera, this->selectMaterialResource.get());
                        base += (int)batch.second->GetDrawers().size();
                    }
                } 
            });
            sceneCamera->SetFramebuffer(sceneFramebufferResource);

            int x = (int)(ImGui::GetMousePos().x - ImGui::GetItemRectMin().x);
            int y = (int)(ImGui::GetItemRectSize().y - ImGui::GetMousePos().y + ImGui::GetItemRectMin().y);
            int id;
            selectFramebufferResource->ReadPixels(&id, x, y);
            if (id) {
                GetSelected() = drawers[id - 1]->GetGameObject();
            } else {
                GetSelected() = nullptr;
            }
        } catch(...) {}
    }
}