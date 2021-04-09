#pragma once

class SceneDialog {
public:
    static SceneDialog &GetInstance() { static SceneDialog sceneDialog; return sceneDialog; }

private:
    SceneDialog() : selected(-1) {}

    int selected;
    std::string arg;
    
public:
    SceneDialog(const SceneDialog &) = delete;
    void operator=(const SceneDialog &) = delete;

    void CreateImGui();
};