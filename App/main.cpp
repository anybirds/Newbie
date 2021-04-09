#include <string>
#include <iostream>
#include <filesystem>

#include <Engine.hpp>

using namespace std;
using namespace glm;
using namespace Engine;

int main(int argc, char **argv) { 
    // type init
    type_init();
    
    // find the project file 
    string pdir(".."); // relative path to the project file is fixed
    string pfile;
    for (auto &p : filesystem::directory_iterator(pdir)) {
        if (p.path().extension().string() == ".json") {
            pfile = p.path().string();
            break;
        }
    }
    
    // create OpenGL context and Window
    Window &window = Window::GetInstance();
    
    // load the Project
    if (!Project::Load(pfile)) {
        return 0;
    }
    Project &project = Project::GetInstance();
    project.SetUseDefaultFramebuffer(true); // render to default framebuffer
    window.SetName(project.GetName());

    // load the start Scene
    if (!Scene::Load(project.GetSetting()->GetStartScene())) {
        return 0;
    }
    Scene &scene = Scene::GetInstance();

    // game loop
    Time::Init();
    scene.Start();
    while (!window.ShouldClose()) {
        Time::Tick();
        
        scene.Refresh();
        scene.Update();
        scene.Render();

        scene.Delete();

		window.SwapBuffers();
        window.PollEvents();
    }

    return 0;
}
