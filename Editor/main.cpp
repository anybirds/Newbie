#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <icons/IconsFontAwesome4.h>

#include <MainMenuBar.hpp>
#include <GamePanel.hpp>
#include <ScenePanel.hpp>

#include <Graphics/Window.hpp>

using namespace std;
using namespace Engine;

int main(int argc, char **argv) {
    // type init
    try {
        type_init();
    } catch (...) {
        return 0;
    }
    
    // create OpenGL context and Window
    Window &window = Window::GetInstance();
    window.SetName("Newbie");
    
    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.Fonts->AddFontFromFileTTF("c:/Newbie/Editor/Fonts/NotoSansCJKkr-hinted/NotoSansCJKkr-Regular.otf", 20.0f, NULL, io.Fonts->GetGlyphRangesKorean());

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 18.0f;
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromFileTTF("c:/Newbie/Editor/Icons/fontawesome-webfont.ttf", 18.0f, &config, icon_ranges);
    
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.GetGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    MainMenuBar mainMenuBar;
    GamePanel gamePanel;
    ScenePanel scenePanel;

    while (!window.ShouldClose()) {
        window.PollEvents();

        // start the imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // menu bar
        mainMenuBar.CreateImGui();
        gamePanel.CreateImGui();
        scenePanel.CreateImGui();
        
        ImGui::ShowDemoWindow();

        // render
        ImGui::Render();
        glViewport(0, 0, window.GetFramebufferWidth(), window.GetFramebufferHeight());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.SwapBuffers();
    }

    window.Destroy();

    return 0;
}
