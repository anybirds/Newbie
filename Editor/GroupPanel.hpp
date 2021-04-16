#pragma once

#include <string>

#include <GameObject.hpp>
#include <Group.hpp>

class GroupPanel {
public:
    static GroupPanel &GetInstance() { static GroupPanel GroupPanel; return GroupPanel; }

private:
    GroupPanel() : open(true) {}

    bool open;
    std::string rename;
    Engine::GameObject *selectedGameObject;
    Engine::GameObject *renamedGameObject;
    Engine::Group *selectedGroup;
    Engine::Group *renamedGroup;
    
public:
    GroupPanel(const GroupPanel &) = delete;
    void operator=(const GroupPanel &) = delete;

    bool IsOpen() { return open; }
    void SetOpen(bool open) { this->open = open; }
    void Toggle() { open ^= true; }
    
    void CreateImGui();
};