#pragma once

class ScenePanel {
public:
    static ScenePanel &GetInstance() { static ScenePanel scenePanel; return scenePanel; }

private:
    ScenePanel() {}

public:
    ScenePanel(const ScenePanel &) = delete;
    void operator=(const ScenePanel &) = delete;

    void CreateImGui();
};
