#pragma once

#include <Panel.hpp>
#include <GameObject.hpp>

class HierarchyPanel : public Panel {
public:
    static HierarchyPanel &GetInstance() { static HierarchyPanel hierarchyPanel; return hierarchyPanel; }

private:
    HierarchyPanel() : Panel("Hierarchy"), menu(false), hovered(nullptr), copyed(nullptr) {}

    bool menu;
    GameObject *hovered;
    GameObject *copyed;

    void ShowIcon(GameObject *gameObject);

public:
    HierarchyPanel(const HierarchyPanel &) = delete;
    HierarchyPanel &operator=(const HierarchyPanel &) = delete;

    virtual void ShowContents();
    void Clear();
};