#pragma once

class GroupPanel {
public:
    static GroupPanel &GetInstance() { static GroupPanel GroupPanel; return GroupPanel; }

private:
    GroupPanel() : open(true) {}

    bool open;
    
public:
    GroupPanel(const GroupPanel &) = delete;
    void operator=(const GroupPanel &) = delete;

    bool IsOpen() { return open; }
    void SetOpen(bool open) { this->open = open; }
    void Toggle() { open ^= true; }
    
    void CreateImGui();
};