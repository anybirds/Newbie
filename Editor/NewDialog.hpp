#pragma once

#include <functional>

#include <Dialog.hpp>

class NewDialog : public Dialog {
public:
    static NewDialog &GetInstance() { static NewDialog newDialog; return newDialog; }

private:
    NewDialog() : Dialog("New") { width = 480.0f; }

    std::string name;
    std::string location;
    std::function<bool(const std::string &)> callback;

public:
    NewDialog(const NewDialog &) = delete;
    NewDialog &operator=(const NewDialog &) = delete;

    virtual void ShowContents() override;

    void SetCallback(const std::function<bool(const std::string &)> &callback) { this->callback = callback; }
};