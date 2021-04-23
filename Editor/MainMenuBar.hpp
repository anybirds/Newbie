#pragma once

#include <Widget.hpp>

class MainMenuBar : public Widget {
public:
    static MainMenuBar &GetInstance() { static MainMenuBar mainMenuBar; return mainMenuBar; }

private:
    MainMenuBar() {}

public:
    MainMenuBar(const MainMenuBar &) = delete;
    MainMenuBar &operator=(const MainMenuBar &) = delete;

    virtual void Show() override;
};