#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Time.hpp>

using namespace Engine;

void Time::Init() {
    dt = 0.0;
    prev = glfwGetTime();
}

void Time::Tick() {
	double curr = glfwGetTime();
    dt = curr - prev;
    prev = curr;
}
