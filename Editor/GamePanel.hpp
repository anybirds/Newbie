#pragma once

#include <memory>

#include <Panel.hpp>

class ATexture;
class AFramebuffer;
class Framebuffer;

class GamePanel : public Panel {
public:
    static GamePanel &GetInstance() { static GamePanel gamePanel; return gamePanel; }

private:
    GamePanel();

    ATexture *gameTexture;
    AFramebuffer *gameFramebuffer;
    std::shared_ptr<Framebuffer> gameFramebufferResource;

public:
    GamePanel(const GamePanel &) = delete;
    GamePanel &operator=(const GamePanel &) = delete;

    virtual void ShowContents() override;
    virtual void Update() override;
    void Close();
};