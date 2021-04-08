#pragma once 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include <EngineExport.hpp>

namespace Engine {
    /*
    Abstraction of a cross-platform window.
    */
    class ENGINE_EXPORT Window final {
    public:
        static Window &GetInstance() { static Window window; return window; }
    
    private:
        static void ErrorCallback(int error, const char *description);
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
        int GetFramebufferWidth() const { return fwidth; }
        int GetFramebufferHeight() const { return fheight; }
        GLFWwindow *GetGlfwWindow() { return window; }

        void SetName(const std::string &name);

        bool ShouldClose();
        void SwapBuffers();
        void PollEvents();
        void Destroy();
    };
}