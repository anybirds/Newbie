#pragma once

#include <memory>

#include <Panel.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Framebuffer.hpp>

class GamePanel : public Panel {
public:
    static GamePanel &GetInstance() { static GamePanel gamePanel; return gamePanel; }

private:
    GamePanel();

    Engine::ATexture *gameTexture;
    Engine::AFramebuffer *gameFramebuffer;
    std::shared_ptr<Engine::Framebuffer> gameFramebufferResource;

public:
    GamePanel(const GamePanel &) = delete;
    void operator=(const GamePanel &) = delete;

    virtual void ShowContents() override;
    void Render();
    void Close();
};