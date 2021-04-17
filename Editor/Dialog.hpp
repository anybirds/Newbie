#pragma once

#include <Widget.hpp>

class Dialog : public Widget {
protected:
    float width;
    std::string title;
    ImGuiWindowFlags flags;

public:
    Dialog(const std::string &title) : title(title), flags(ImGuiWindowFlags_None) {}

    virtual void Show() override;
    virtual void ShowContents() = 0;
    virtual void Open();
};