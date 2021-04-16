#pragma once

#include <Dialog.hpp>

class SceneDialog : public Dialog {
public:
    static SceneDialog &GetInstance() { static SceneDialog sceneDialog; return sceneDialog; }

private:
    SceneDialog() : Dialog("Open Scene"), newScene(false) { width = 480.0f; }

    bool newScene;
    std::string name;
    std::string location;
    
public:
    SceneDialog(const SceneDialog &) = delete;
    void operator=(const SceneDialog &) = delete;

    virtual void ShowContents() override;
    void SetNewScene() { newScene = true; }
};