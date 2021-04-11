#pragma once

#include <Graphics/Camera.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Framebuffer.hpp>
#include <GameObject.hpp>

class ScenePanel {
public:
    static ScenePanel &GetInstance() { static ScenePanel scenePanel; return scenePanel; }

private:
    ScenePanel();

    bool open;

    Engine::ATexture *sceneTexture;
    Engine::AFramebuffer *sceneFramebuffer;
    Engine::GameObject *sceneCamera;
    Engine::Camera *cam;

public:
    ScenePanel(const ScenePanel &) = delete;
    void operator=(const ScenePanel &) = delete;

    bool IsOpen() { return open; }
    void SetOpen(bool open) { this->open = open; }
    void Toggle() { open ^= true; }
    
    void CreateImGui();
    void Close();
};
