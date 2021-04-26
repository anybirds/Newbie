#pragma once

#include <Widget.hpp>

class Panel : public Widget {
protected:
    bool open;
    bool focus;
    std::string title;
    ImGuiWindowFlags flags;

public:
    Panel(const std::string &title) : open(false), focus(false), title(title), flags(ImGuiWindowFlags_None) {}

    bool IsOpen() { return open; }
    void SetOpen(bool open) { this->open = open; }
    void Toggle() { open ^= true; }

    virtual void Show() override;
    virtual void ShowContents() = 0;
    virtual void Update() {}
};
