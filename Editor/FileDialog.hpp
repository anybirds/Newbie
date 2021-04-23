#pragma once

#include <functional>

#include <Dialog.hpp>

class FileDialog : public Dialog {
public:
    static FileDialog &GetInstance() { static FileDialog fileDialog; return fileDialog; }

private:
    FileDialog() : Dialog("Open"), folder(false), dir(".") { width = 480.0f; }

    bool folder;
    std::string dir;
    std::string arg;
    std::function<void(const std::string &)> callback;

public:
    FileDialog(const FileDialog &) = delete;
    FileDialog &operator=(const FileDialog &) = delete;

    virtual void ShowContents() override;

    bool IsFolderDialog() const { return folder; }
    void SetFileDialog() { this->folder = false; }
    void SetFolderDialog() { this->folder = true; }
    void SetCallback(const std::function<void(const std::string &)> &callback) { this->callback = callback; }
};
