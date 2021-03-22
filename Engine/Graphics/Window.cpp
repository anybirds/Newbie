#include <iostream>

#include <Graphics/Window.hpp>

using namespace std;
using namespace Engine;

Window::Window() {
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

    // glew init
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK) {
        // Initializing GLEW failed
        cerr << '[' << __FUNCTION__ << ']' << " message: " << glewGetErrorString(glew_error) << '\n';
        throw exception();
    }
}

Window::~Window() {
	glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::SetName(const string &name) {
    this->name = name;
    glfwSetWindowTitle(window, name.c_str());
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
