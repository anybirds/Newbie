#pragma once

#include <memory>

#include <Graphics/Texture.hpp>
#include <Graphics/Framebuffer.hpp>

class GamePanel {
public:
    static GamePanel &GetInstance() { static GamePanel gamePanel; return gamePanel; }

private:
    GamePanel();
    
    bool open;

    Engine::ATexture *gameTexture;
    Engine::AFramebuffer *gameFramebuffer;
    std::shared_ptr<Engine::Framebuffer> gameFramebufferResource;

public:
    GamePanel(const GamePanel &) = delete;
    void operator=(const GamePanel &) = delete;

    bool IsOpen() { return open; }
    void SetOpen(bool open) { this->open = open; }
    void Toggle() { open ^= true; }

    void CreateImGui();
    void Close();
};