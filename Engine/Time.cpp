#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Time.hpp>

using namespace Engine;

void Time::Init() {
    Time &time = GetInstance();
    time.dt = 0.0;
    time.prev = glfwGetTime();
}

void Time::Tick() {
    Time &time = GetInstance();
	double curr = glfwGetTime();
    time.dt = curr - time.prev;
    time.prev = curr;
}
