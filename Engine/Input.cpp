#include <Input.hpp>
#include <Graphics/Window.hpp>

using namespace Engine;

void Input::Init() {
    glfwGetCursorPos(Window::GetInstance().GetGlfwWindow(), &mouseCursorX, &mouseCursorY);
}

void Input::Update() {
    double prevX = mouseCursorX;
    double prevY = mouseCursorY;
    glfwGetCursorPos(Window::GetInstance().GetGlfwWindow(), &mouseCursorX, &mouseCursorY);
    deltaMouseCursorX = mouseCursorX - prevX;
    deltaMouseCursorY = mouseCursorY - prevY;
}

void Input::SetMouseCursorMode(int mode) {
    Window &window = Window::GetInstance();
    switch (mode) {
        case MOUSE_CURSOR_NORMAL:
            glfwSetInputMode(window.GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case MOUSE_CURSOR_HIDDEN:
            glfwSetInputMode(window.GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        case MOUSE_CURSOR_DISABLED:
            glfwSetInputMode(window.GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        default:
            break;
    }
}