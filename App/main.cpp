#include <string>
#include <iostream>
#include <filesystem>

#include <Engine.hpp>
#include <Window.hpp>

using namespace std;
using namespace glm;
using namespace Engine;

int main(int argc, char **argv) { 
    // type init
    type_init();
    
    // get the name of the Project
    string name;
    for (const auto &file : filesystem::directory_iterator(".")) {
		const string &str = file.path().filename().string();
		std::string::size_type i = str.rfind('.');
		if (i != std::string::npos && !str.substr(i + 1).compare("json")) {
            name = str;
            break;
		}
	}
    if (name.empty()) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to find project file."  << '\n';
        return 0;
    }
    
    // create OpenGL context and Window
    try {
        Window window(name);
    } catch(...) {
        return 0;
    }

    // load the Project
    if (!Project::Load(name)) {
        return 0;
    }
    
    // load the start Scene
    Project &project = Project::GetInstance();
    if (!Scene::Load(project.setting.GetStartSceneName())) {
        return 0;
    }
    
    // game loop
    Time::Init();
    Scene::Start();
    while (!window.ShouldClose()) {
        Time::Tick();
        
        Scene::Update();
        Scene::Render();

		window.SwapBuffers();
        window.PollEvents();
    }

    return 0;
}
