#pragma once 

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Engine {
    /*
    Abstraction of a cross-platform window.
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
}