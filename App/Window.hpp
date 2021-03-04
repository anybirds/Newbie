#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
Window

Abstraction of a cross-platform window that appears in the monitor.
 */
class Window final {
public:
    GLFWwindow *window;
    std::string name;
    int width;
    int height;

public:
    Window(const std::string &name);
    Window(const std::string &name, int width, int height);
    ~Window();

    bool ShouldClose();
    void SwapBuffers();
    void PollEvents();
};

#endif
