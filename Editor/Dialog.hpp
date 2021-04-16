#pragma once

#include <Widget.hpp>

class Dialog : public Widget {
protected:
    float width;
    std::string title;

public:
    Dialog(const std::string &title) : title(title) {}

    virtual void Show() override;
    virtual void ShowContents() = 0;
};