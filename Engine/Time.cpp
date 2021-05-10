#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Time.hpp>

void Time::Init() {
    dt = 0.0;
    prev = glfwGetTime();
}

void Time::Update() {
	double curr = glfwGetTime();
    dt = curr - prev;
    prev = curr;
}
