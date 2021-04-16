#pragma once

#include <string>

#include <Asset.hpp>

class AssetPanel {
public:
    static AssetPanel &GetInstance() { static AssetPanel AssetPanel; return AssetPanel; }

private:
    AssetPanel() : open(true) {}

    bool open;
    std::string rename;
    Engine::Asset *selectedAsset;
    Engine::Asset *renamedAsset;
    
public:
    AssetPanel(const AssetPanel &) = delete;
    void operator=(const AssetPanel &) = delete;

    bool IsOpen() { return open; }
    void SetOpen(bool open) { this->open = open; }
    void Toggle() { open ^= true; }
    
    void CreateImGui();
};