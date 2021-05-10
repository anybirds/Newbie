#pragma once

#include <Panel.hpp>
#include <Asset.hpp>
#include <Prefab.hpp>

class AssetPanel : public Panel {
public:
    static AssetPanel &GetInstance() { static AssetPanel assetPanel; return assetPanel; }

private:
    AssetPanel() : Panel("Asset") {}

    void ShowIcon(Asset *asset);

public:
    AssetPanel(const AssetPanel &) = delete;
    AssetPanel &operator=(const AssetPanel &) = delete;
    
    virtual void ShowContents() override;
    void ShowPreviewOff();
};