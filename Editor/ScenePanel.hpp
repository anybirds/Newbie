#pragma once

#include <memory>

#include <Panel.hpp>
#include <imgui/ImGuizmo.h>

class Transform;
class Camera;
class ATexture;
class AFramebuffer;
class Framebuffer;
class AShader;
class AMaterial;
class Material;
class Drawer;

class ScenePanel : public Panel {
private:
    static const std::string &GetWhiteVertexShader();
    static const std::string &GetWhiteFragmentShader();
    static const std::string &GetOutlineVertexShader();
    static const std::string &GetOutlineFragmentShader();
    static const std::string &GetSelectVertexShader();
    static const std::string &GetSelectFragmentShader();
    
public:
    static ScenePanel &GetInstance() { static ScenePanel scenePanel; return scenePanel; }

private:
    ScenePanel();

    Transform *sceneTransform;
    Camera *sceneCamera;
    ATexture *sceneTexture;
    AFramebuffer *sceneFramebuffer;
    std::shared_ptr<Framebuffer> sceneFramebufferResource;

    ATexture *whiteTexture;
    AFramebuffer *whiteFramebuffer;
    AShader *whiteVertexShader;
    AShader *whiteFragmentShader;
    AMaterial *whiteMaterial;
    std::shared_ptr<Material> whiteMaterialResource;
    std::shared_ptr<Framebuffer> whiteFramebufferResource;
    
    ATexture *copyTexture;
    AFramebuffer *copyFramebuffer;
    std::shared_ptr<Framebuffer> copyFramebufferResource;

    Transform *outlineTransform;
    Camera *outlineCamera;
    Drawer *outlineDrawer;
    AShader *outlineVertexShader;
    AShader *outlineFragmentShader;
    AMaterial *outlineMaterial;
    std::shared_ptr<Material> outlineMaterialResource;
    
    ATexture *selectTexture;
    AFramebuffer *selectFramebuffer;
    AShader *selectVertexShader;
    AShader *selectFragmentShader;
    AMaterial *selectMaterial;
    std::shared_ptr<Material> selectMaterialResource;
    std::shared_ptr<Framebuffer> selectFramebufferResource;

    bool control;
    float moveSpeed;
    float rotateSpeed;

    ImGuizmo::OPERATION gizmoOperation;
    ImGuizmo::MODE gizmoMode;
    
public:
    ScenePanel(const ScenePanel &) = delete;
    ScenePanel &operator=(const ScenePanel &) = delete;

    virtual void ShowContents() override;
    virtual void Update() override;
    void Close();
};
