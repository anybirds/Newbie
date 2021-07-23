#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <icons/IconsFontAwesome5.h>

#include <MainMenuBar.hpp>
#include <GamePanel.hpp>
#include <ScenePanel.hpp>
#include <AssetPanel.hpp>
#include <HierarchyPanel.hpp>
#include <InspectorPanel.hpp>

#include <Project.hpp>
#include <Graphics/Window.hpp>
#include <Time.hpp>
#include <Input.hpp>

using namespace std;

int main(int argc, char **argv) {
    // type init
    try {
        type_init();
    } catch (...) {
        return 0;
    }
    
    // create OpenGL context and Window
    Window &window = Window::GetInstance();
    window.SetTitle("Newbie");
    
    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigWindowsMoveFromTitleBarOnly = true; // https://github.com/ocornut/imgui/issues/899
    io.Fonts->AddFontFromFileTTF(
        (string(NEWBIE_PATH) + "/Editor/Fonts/NotoSansCJKkr-hinted/NotoSansCJKkr-Regular.otf").c_str(), 
        20.0f, NULL, io.Fonts->GetGlyphRangesKorean());

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 16.0f;
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromFileTTF(
        (string(NEWBIE_PATH) + "/Editor/Icons/fa-solid-900.ttf").c_str(), 
        16.0f, &config, icon_ranges);
    
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.GetGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    MainMenuBar &mainMenuBar = MainMenuBar::GetInstance();
    GamePanel &gamePanel = GamePanel::GetInstance();
    ScenePanel &scenePanel = ScenePanel::GetInstance();
    AssetPanel &assetPanel = AssetPanel::GetInstance();
    HierarchyPanel &hierarchyPanel = HierarchyPanel::GetInstance();
    InspectorPanel &inspectorPanel = InspectorPanel::GetInstance();

    scenePanel.SetOpen(true);
    assetPanel.SetOpen(true);
    hierarchyPanel.SetOpen(true);
    inspectorPanel.SetOpen(true);
    
    Time &time = Time::GetInstance();
    time.Init();
    Input &input = Input::GetInstance();
    input.Init();

    while (!window.ShouldClose()) {
        window.PollEvents();
        time.Update();
        input.Update();

        // start the imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Widget::Update();
        mainMenuBar.Show();
        gamePanel.Show();
        scenePanel.Show();
        assetPanel.Show();
        hierarchyPanel.Show();
        inspectorPanel.Show();

        ImGui::ShowDemoWindow();

        // render
        ImGui::Render();
        glViewport(0, 0, window.GetFramebufferWidth(), window.GetFramebufferHeight());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.SwapBuffers();
    }
    
    hierarchyPanel.Clear();
    Project::GetInstance().Close();
    
    gamePanel.Close();
    scenePanel.Close();
    window.Close();

    return 0;
}
