#pragma once

#include <algorithm>
#include <string>
#include <functional>

class FileDialog {
public:
    static FileDialog &GetInstance() { static FileDialog fileDialog; return fileDialog; }

private:
    FileDialog() : selected(-1), dir("."), folder(false) { std::fill(buf, buf + 256, 0); }

    int selected;
    std::string arg;
    char buf[256];
    std::string dir;

    bool folder;
    std::function<void(const std::string &)> callback;

public:
    FileDialog(const FileDialog &) = delete;
    void operator=(const FileDialog &) = delete;

    void CreateImGui();
    
    bool IsFolderDialog() const { return folder; }
    void SetFolderDialog(bool folder) { this->folder = folder; }
    void SetCallback(const std::function<void(const std::string &)> &callback) { this->callback = callback; }
};
