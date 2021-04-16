#pragma once

#include <Widget.hpp>

class Panel : public Widget {
protected:
    bool open;
    std::string title;

public:
    Panel(const std::string &title) : open(false), title(title) {}

    bool IsOpen() { return open; }
    void SetOpen(bool open) { this->open = open; }
    void Toggle() { open ^= true; }

    virtual void Show() override;
    virtual void ShowContents() = 0;
};
