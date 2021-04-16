#pragma once

#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <Icons/IconsFontAwesome5.h>

class Widget {
protected:
    bool rename;
    void *selected;

public:
    Widget() : rename(false), selected(nullptr) {}
    virtual ~Widget() {}
    virtual void Show() = 0;    

    void ShowRename(std::string &str); 
    void UpdateRename();
};