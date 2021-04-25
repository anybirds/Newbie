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

    bool running;
    bool paused;
    Engine::ATexture *gameTexture;
    Engine::AFramebuffer *gameFramebuffer;
    std::shared_ptr<Engine::Framebuffer> gameFramebufferResource;

public:
    GamePanel(const GamePanel &) = delete;
    GamePanel &operator=(const GamePanel &) = delete;

    bool IsGameRunning() { return running; }
    virtual void ShowContents() override;
    void ShowPlayPause();
    virtual void Update() override;
    void Close();
};