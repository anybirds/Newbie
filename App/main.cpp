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
    Project &project = Project::GetInstance();
    if (!project.Load(pfile)) {
        return 0;
    }
    window.SetTitle(project.GetName());

    // load the start Scene
    Scene &scene = Scene::GetInstance();
    if (!scene.LoadImmediate(project.GetSetting()->GetStartScene())) {
        return 0;
    }

    
    Time &time = Time::GetInstance();
    time.Init();
    Input &input = Input::GetInstance();
    input.Init();

    // game loop
    while (!window.ShouldClose()) {
        window.PollEvents();
        time.Update();
        input.Update();

        scene.Loop();

		window.SwapBuffers();
    }

    window.Close();

    return 0;
}
