#include <string>
#include <iostream>
#include <filesystem>

#include <Engine.hpp>
#include <Graphics/Window.hpp>

using namespace std;
using namespace glm;
using namespace Engine;

int main(int argc, char **argv) {
    // type init
    try {
        type_init();
    } catch (...) {
        return 0;
    }
    
    // find the project file 
    string pdir("c:/Newbie/Samples/Lofi/");
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
    window.SetName(project.GetName());

    // load the start Scene
    if (!Scene::Load(project.GetSetting()->GetStartSceneIndex())) {
        return 0;
    }
    Scene &scene = Scene::GetInstance();

    // game loop
    Time::Init();
    scene.Start();
    while (!window.ShouldClose()) {
        Time::Tick();
        
        scene.Update();
        scene.Render();

		window.SwapBuffers();
        window.PollEvents();
    }

    return 0;
}
