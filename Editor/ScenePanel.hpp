#pragma once

#include <Panel.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Framebuffer.hpp>
#include <GameObject.hpp>

class ScenePanel : public Panel {
public:
    static ScenePanel &GetInstance() { static ScenePanel scenePanel; return scenePanel; }

private:
    ScenePanel();

    Engine::ATexture *sceneTexture;
    Engine::AFramebuffer *sceneFramebuffer;
    Engine::GameObject *sceneCamera;
    Engine::Camera *cam;

public:
    ScenePanel(const ScenePanel &) = delete;
    void operator=(const ScenePanel &) = delete;

    virtual void ShowContents() override;
    void Close();
};
