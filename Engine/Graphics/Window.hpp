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
        static void ErrorCallback(int error, const char *description);
        static void FramebufferResizeCallback(GLFWwindow *glfwWindow, int fwidth, int fheight);
        
    private:
        Window();

        GLFWwindow *window; 
        std::string title;
        int width;
        int height;
        int fwidth; // framebuffer width
        int fheight; // framebuffer height
        int mwidth; // monitor width
        int mheight; // monitor height

    public:
        Window(const Window &) = delete;
        void operator=(const Window &) = delete;

        const std::string &GetTitle() const { return title; }
        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        int GetFramebufferWidth() const { return fwidth; }
        int GetFramebufferHeight() const { return fheight; }
        int GetMonitorWidth() const { return mwidth; }
        int GetMonitorHeight() const { return mheight; }
        GLFWwindow *GetGlfwWindow() { return window; }

        void SetTitle(const std::string &title);

        bool ShouldClose();
        void SwapBuffers();
        void PollEvents();
        void Close();
    };
}