#pragma once

#include <unordered_set>

#include <Dialog.hpp>

class ProjectDialog : public Dialog {
private:
    static const std::string &GetProjectsFile();
    static const std::string &GetEmptyProject();

public:
    static ProjectDialog &GetInstance() { static ProjectDialog projectDialog; return projectDialog; }

private:
    ProjectDialog();
    
    bool newProject;
    std::string name;
    std::string location;
    std::unordered_set<std::string> projects;
    
    std::string GetProjectFile(const std::string &dir) const;
    void AddProject(const std::string &dir);
    void RemoveProject(const std::string &dir);
    void LoadProject(const std::string &dir);

public:
    ProjectDialog(const ProjectDialog &) = delete;
    ProjectDialog &operator=(const ProjectDialog &) = delete;

    void ShowContents();
    void SetNewProject() { newProject = true; }
};