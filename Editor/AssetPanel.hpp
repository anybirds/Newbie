#pragma once

class AssetPanel {
public:
    static AssetPanel &GetInstance() { static AssetPanel AssetPanel; return AssetPanel; }

private:
    AssetPanel() : open(true) {}

    bool open;
    
public:
    AssetPanel(const AssetPanel &) = delete;
    void operator=(const AssetPanel &) = delete;

    bool IsOpen() { return open; }
    void SetOpen(bool open) { this->open = open; }
    void Toggle() { open ^= true; }
    
    void CreateImGui();
};