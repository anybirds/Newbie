#pragma once

#include <utility>
#include <nlohmann/json.hpp>

#include <Panel.hpp>
#include <GameObject.hpp>

class HierarchyPanel : public Panel {
public:
    static HierarchyPanel &GetInstance() { static HierarchyPanel hierarchyPanel; return hierarchyPanel; }

private:
    HierarchyPanel() : Panel("Hierarchy"), menu(false) {}

    bool menu;
    nlohmann::json copyed;

public:
    HierarchyPanel(const HierarchyPanel &) = delete;
    HierarchyPanel &operator=(const HierarchyPanel &) = delete;

    virtual void ShowContents();
    void Clear();
};