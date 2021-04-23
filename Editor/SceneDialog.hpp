#pragma once

#include <Dialog.hpp>

class SceneDialog : public Dialog {
private:
    static const std::string &GetEmptyScenePath();

public:
    static SceneDialog &GetInstance() { static SceneDialog sceneDialog; return sceneDialog; }

private:
    SceneDialog() : Dialog("Open Scene"), newScene(false) { flags = ImGuiWindowFlags_MenuBar; width = 600.0f; }

    bool newScene;
    std::string name;
    std::string location;
    
public:
    SceneDialog(const SceneDialog &) = delete;
    SceneDialog &operator=(const SceneDialog &) = delete;

    virtual void ShowContents() override;
    void SetNewScene() { newScene = true; }
};