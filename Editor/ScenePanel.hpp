#pragma once

#include <memory>

#include <Panel.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Framebuffer.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Material.hpp>
#include <GameObject.hpp>

#include "imgui/ImGuizmo.h"

class ScenePanel : public Panel {
private:
    static const std::string &GetSelectVertexShader();
    static const std::string &GetSelectFragmentShader();

public:
    static ScenePanel &GetInstance() { static ScenePanel scenePanel; return scenePanel; }

private:
    ScenePanel();

    GameObject *sceneCamera;
    Camera *camera;

    ATexture *sceneTexture;
    AFramebuffer *sceneFramebuffer;
    
    ATexture *selectTexture;
    AFramebuffer *selectFramebuffer;
    AShader *selectVertexShader;
    AShader *selectFragmentShader;
    AMaterial *selectMaterial;

    std::shared_ptr<Material> selectMaterialResource;
    std::shared_ptr<Framebuffer> sceneFramebufferResource;
    std::shared_ptr<Framebuffer> selectFramebufferResource;

    bool control;
    float moveSpeed;
    float rotateSpeed;

    ImGuizmo::OPERATION gizmoOperation;
    ImGuizmo::MODE gizmoMode;

    bool clicked;
    int clickX;
    int clickY;
    
public:
    ScenePanel(const ScenePanel &) = delete;
    ScenePanel &operator=(const ScenePanel &) = delete;

    virtual void ShowContents() override;
    virtual void Update() override;
    void Close();
};
