#pragma once

#include <FileDialog.hpp>

class MainMenuBar {
public:
    static MainMenuBar &GetInstance() { static MainMenuBar mainMenuBar; return mainMenuBar; }

private:
    MainMenuBar() {}

public:
    MainMenuBar(const MainMenuBar &) = delete;
    void operator=(const MainMenuBar &) = delete;
    
    void CreateImGui();
};