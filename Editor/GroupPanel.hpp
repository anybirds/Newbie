#pragma once

#include <Panel.hpp>
#include <GameObject.hpp>
#include <Group.hpp>

class GroupPanel : public Panel {
public:
    static GroupPanel &GetInstance() { static GroupPanel groupPanel; return groupPanel; }

private:
    GroupPanel() : Panel("Group") {}
    
public:
    GroupPanel(const GroupPanel &) = delete;
    void operator=(const GroupPanel &) = delete;

    virtual void ShowContents();
};