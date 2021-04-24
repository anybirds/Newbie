#pragma once

#include <Panel.hpp>

class HierarchyPanel : public Panel {
public:
    static HierarchyPanel &GetInstance() { static HierarchyPanel hierarchyPanel; return hierarchyPanel; }

private:
    HierarchyPanel() : Panel("Hierarchy") {}
    
public:
    HierarchyPanel(const HierarchyPanel &) = delete;
    HierarchyPanel &operator=(const HierarchyPanel &) = delete;

    virtual void ShowContents();
};