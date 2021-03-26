#include <iostream>

#include <Graphics/Window.hpp>

using namespace std;
using namespace Engine;

void Window::FramebufferResizeCallback(GLFWwindow *glfwWindow, GLsizei fwidth, GLsizei fheight) {
    glViewport(0, 0, fwidth, fheight);
    
    Window &window = GetInstance();
    glfwGetWindowSize(glfwWindow, &window.width, &window.height);
    window.fwidth = fwidth;
    window.fheight = fheight;
}

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

    window = glfwCreateWindow(mode->width, mode->height, name.c_str(), NULL, NULL);
    if (!window) {
        cerr << '[' << __FUNCTION__ << ']' << " window creation failed."  << '\n';
        throw exception();
    }
    glfwMakeContextCurrent(window);
    glfwMaximizeWindow(window);
    glfwGetWindowSize(window, &width, &height);
    glfwGetFramebufferSize(window, &fwidth, &fheight);
    
    // glew init
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK) {
        // Initializing GLEW failed
        cerr << '[' << __FUNCTION__ << ']' << " message: " << glewGetErrorString(glew_error) << '\n';
        throw exception();
    }

    // set callbacks
    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
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
