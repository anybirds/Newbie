#pragma once

#include <Widget.hpp>

class Type;

class Panel : public Widget {
protected:
    bool open;
    std::string title;
    ImGuiWindowFlags flags;

public:
    Panel(const std::string &title) : open(false), title(title), flags(ImGuiWindowFlags_HorizontalScrollbar) {}

    bool IsOpen() { return open; }
    void SetOpen(bool open) { this->open = open; }
    void Toggle() { open ^= true; }

    virtual void Show() override;
    virtual void ShowContents() = 0;
    virtual void Update() {}
    
    const char *GetIconCharacter(Type *type);
    void ShowIcon(Type *type);
};
