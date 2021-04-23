#pragma once

#include <memory>

#include <Panel.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Framebuffer.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Material.hpp>
#include <GameObject.hpp>

class ScenePanel : public Panel {
private:
    static const std::string &GetSelectVertexShader();
    static const std::string &GetSelectFragmentShader();

public:
    static ScenePanel &GetInstance() { static ScenePanel scenePanel; return scenePanel; }

private:
    ScenePanel();

    Engine::GameObject *sceneCamera;
    Engine::Camera *camera;

    Engine::ATexture *sceneTexture;
    Engine::AFramebuffer *sceneFramebuffer;
    
    Engine::ATexture *selectTexture;
    Engine::AFramebuffer *selectFramebuffer;
    Engine::AShader *selectVertexShader;
    Engine::AShader *selectFragmentShader;
    Engine::AMaterial *selectMaterial;

    std::shared_ptr<Engine::Material> selectMaterialResource;
    std::shared_ptr<Engine::Framebuffer> sceneFramebufferResource;
    std::shared_ptr<Engine::Framebuffer> selectFramebufferResource;

    bool control;
    float moveSpeed;
    float rotateSpeed;

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
