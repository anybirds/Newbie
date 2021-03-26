#pragma once 

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <EngineExport.hpp>

namespace Engine {
    /*
    Abstraction of a cross-platform window.
    */
    class ENGINE_EXPORT Window final {
    public:
        static Window &GetInstance() { static Window window; return window; }
    
    private:
        static void FramebufferResizeCallback(GLFWwindow *glfwWindow, int fwidth, int fheight);
        
    private:
        Window();

        GLFWwindow *window; 
        std::string name;
        int width;
        int height;
        int fwidth; // framebuffer width
        int fheight; // framebuffer height

    public:
        Window(const Window &) = delete;
        void operator=(const Window &) = delete;

        const std::string &GetName() const { return name; }
        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        
        void SetName(const std::string &name);
        
        ~Window();

        bool ShouldClose();
        void SwapBuffers();
        void PollEvents();
    };
}