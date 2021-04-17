#pragma once

#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <Icons/IconsFontAwesome5.h>

class Widget {
protected:
    static void *selected;
    static bool rename;
    static bool renameClicked;

public:
    static void UpdateRename();

public:
    virtual ~Widget() {}
    virtual void Show() = 0;    

    void ShowRename(std::string &str); 
};