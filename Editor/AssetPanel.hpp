#pragma once

#include <Panel.hpp>
#include <Asset.hpp>
#include <GameObject.hpp>

class AssetPanel : public Panel {
public:
    static AssetPanel &GetInstance() { static AssetPanel assetPanel; return assetPanel; }

private:
    AssetPanel() : Panel("Asset"), gameObject(nullptr) {}
    
    Engine::GameObject *gameObject;

public:
    AssetPanel(const AssetPanel &) = delete;
    AssetPanel &operator=(const AssetPanel &) = delete;
    
    virtual void ShowContents() override;
};