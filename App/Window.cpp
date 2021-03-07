#include <iostream>

#include <Window.hpp>

using namespace std;

Window::Window(const std::string &name) {
    if (!glfwInit()) {
        cerr << '[' << __FUNCTION__ << ']' << " glfw init failed."  << '\n';
        throw exception();
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // full-screen window
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    width = mode->width;
    height = mode->height;

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!window) {
        cerr << '[' << __FUNCTION__ << ']' << " window creation failed."  << '\n';
        throw exception();
    }
    glfwMakeContextCurrent(window);
    glfwMaximizeWindow(window);
}

Window::Window(const std::string &name, int width, int height)
	: name(name), width(width), height(height) {
    if (!glfwInit()) {
        cerr << '[' << __FUNCTION__ << ']' << " glfw init failed."  << '\n';
        throw exception();
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!window) {
        cerr << '[' << __FUNCTION__ << ']' << " window creation failed."  << '\n';
        throw exception();
    }
	glfwMakeContextCurrent(window);
}

Window::~Window() {
	glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::ShouldClose() {
	return static_cast<bool>(glfwWindowShouldClose(window));
}

void Window::SwapBuffers() {
    glfwSwapBuffers(window);
}

void Window::PollEvents() {
    glfwPollEvents();
}
