#pragma once

#include <string>

class ProjectDialog {
public:
    static ProjectDialog &GetInstance() { static ProjectDialog ProjectDialog; return ProjectDialog; }

private:
    ProjectDialog() {}
    
    std::string name;
    std::string location;
    
public:
    ProjectDialog(const ProjectDialog &) = delete;
    void operator=(const ProjectDialog &) = delete;

    void CreateImGui();
};