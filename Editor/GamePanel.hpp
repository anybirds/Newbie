#pragma once

class GamePanel {
public:
    static GamePanel &GetInstance() { static GamePanel gamePanel; return gamePanel; }

private:
    GamePanel() {}

public:
    GamePanel(const GamePanel &) = delete;
    void operator=(const GamePanel &) = delete;

    void CreateImGui();
};