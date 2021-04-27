#pragma once

#include <Panel.hpp>
#include <GameObject.hpp>

class HierarchyPanel : public Panel {
public:
    static HierarchyPanel &GetInstance() { static HierarchyPanel hierarchyPanel; return hierarchyPanel; }

private:
    HierarchyPanel() : Panel("Hierarchy"), menu(false), hovered(nullptr), copyed(nullptr) {}
    
    bool menu;
    Engine::GameObject *hovered;
    Engine::GameObject *copyed;

    void ShowIcon(Engine::GameObject *gameObject);

public:
    HierarchyPanel(const HierarchyPanel &) = delete;
    HierarchyPanel &operator=(const HierarchyPanel &) = delete;

    virtual void ShowContents();
    void Close();
};