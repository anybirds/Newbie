#pragma once

#include <nlohmann/json.hpp>

#include <Panel.hpp>

class Type;

class InspectorPanel : public Panel {
public:
    static InspectorPanel &GetInstance() { static InspectorPanel inspectorPanel; return inspectorPanel; }

private:
    InspectorPanel();

    std::string searchAdd;
    bool menu;
    Type *copyedType;
    nlohmann::json copyed;

public:
    InspectorPanel(const InspectorPanel &) = delete;
    InspectorPanel &operator=(const InspectorPanel &) = delete;

    virtual void ShowContents() override;
    void Clear();
};