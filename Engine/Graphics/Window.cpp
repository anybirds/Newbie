#include <iostream>

#include <Graphics/Window.hpp>

using namespace std;

void Window::ErrorCallback(int error, const char *description) {
    cerr << "glfw error " << error << ": " << description << '\n';
}

void Window::FramebufferResizeCallback(GLFWwindow *glfwWindow, GLsizei fwidth, GLsizei fheight) {
    glViewport(0, 0, fwidth, fheight);
    
    Window &window = GetInstance();
    glfwGetWindowSize(glfwWindow, &window.width, &window.height);
    window.fwidth = fwidth;
    window.fheight = fheight;
}

Window::Window() {
    glfwSetErrorCallback(ErrorCallback);
    if (!glfwInit()) {
        cerr << '[' << __FUNCTION__ << ']' << " glfw init failed."  << '\n';
        throw exception();
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    mwidth = mode->width; mheight = mode->height;
    window = glfwCreateWindow(mwidth, mheight, title.c_str(), NULL, NULL);
    if (!window) {
        cerr << '[' << __FUNCTION__ << ']' << " window creation failed."  << '\n';
        throw exception();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // enable vsync
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

void Window::Close() {
	glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::SetTitle(const string &title) {
    this->title = title;
    glfwSetWindowTitle(window, title.c_str());
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
