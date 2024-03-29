#pragma once

#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <Icons/IconsFontAwesome5.h>
#include <nlohmann/json.hpp>

class Entity;
class Type;

class Widget {
protected:
    static Entity *&GetSelected() { static Entity *selected; return selected; }
    static void *&GetLocalSelected() { static void *localSelected; return localSelected; }
    static bool IsSelected() { return GetLocalSelected() || GetSelected(); } 
    static bool &IsRenaming() { static bool renaming; return renaming; }
    static bool &IsClickedRenamingItem() { static bool clicked; return clicked; }
    static Type *&GetCopyedType() { static Type *copyedType; return copyedType; }
    static nlohmann::json &GetCopyed() { static nlohmann::json copyed; return copyed; }

public:
    static void ShowRenamingItem(std::string &str);
    static void Update();
    static void Clear();

public:
    virtual ~Widget() {}

    virtual void Show() = 0;
};