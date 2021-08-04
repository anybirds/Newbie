#pragma once

#include <nlohmann/json.hpp>

#include <Panel.hpp>

class Type;
class Asset;

class AssetPanel : public Panel {
public:
    static AssetPanel &GetInstance() { static AssetPanel assetPanel; return assetPanel; }

private:
    AssetPanel() : Panel("Asset") {}

public:
    AssetPanel(const AssetPanel &) = delete;
    AssetPanel &operator=(const AssetPanel &) = delete;
    
    virtual void ShowContents() override;
    void ShowAsset(Asset *asset);
};