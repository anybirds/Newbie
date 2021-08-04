#pragma once

#include <Widget.hpp>

class MainMenuBar : public Widget {
public:
    static MainMenuBar &GetInstance() { static MainMenuBar mainMenuBar; return mainMenuBar; }

private:
    MainMenuBar() : gamePlaying(false), gamePaused(false) {}

protected:
    bool gamePlaying;
    bool gamePaused;

public:
    MainMenuBar(const MainMenuBar &) = delete;
    MainMenuBar &operator=(const MainMenuBar &) = delete;

    bool IsGamePlaying() const { return gamePlaying; }
    bool IsGamePaused() const { return gamePaused; }
    void StartGamePlay() { gamePlaying = true; gamePaused = false; }
    void StopGamePlay() { gamePlaying = false; gamePaused = false; }
    virtual void Show() override;
    void ShowReload();
    void ShowPlayPause();
};