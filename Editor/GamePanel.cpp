#include <cstdint>
#include <filesystem>

#include <Engine.hpp>
#include <GamePanel.hpp>
#include <AssetPanel.hpp>
#include <MainMenuBar.hpp>

using namespace std;

GamePanel::GamePanel() : Panel("Game") {
    Window &window = Window::GetInstance();

    gameTexture = new ATexture();
    gameTexture->SetWidth(window.GetMonitorWidth());
    gameTexture->SetHeight(window.GetMonitorHeight());
    gameTexture->SetHorizontalWrap(TextureWrapMode::CLAMP_TO_BORDER);
    gameTexture->SetVerticalWrap(TextureWrapMode::CLAMP_TO_BORDER);

    gameFramebuffer = new AFramebuffer();
    gameFramebuffer->SetColorTexture(gameTexture);
    gameFramebuffer->SetUseDepthRenderTexture(true);

    gameFramebufferResource = dynamic_pointer_cast<Framebuffer>(gameFramebuffer->GetResource());
    
    Camera::SetDefaultFramebuffer(gameFramebufferResource);
}

void GamePanel::Close() {
    gameFramebufferResource.reset();
    delete gameFramebuffer;
    delete gameTexture;
}

void GamePanel::Update() {
    glBindFramebuffer(GL_FRAMEBUFFER, gameFramebufferResource->GetId());
    glViewport(0, 0, gameFramebufferResource->GetMaxWidth(), gameFramebufferResource->GetMaxHeight());
    glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Scene &scene = Scene::GetInstance();
    MainMenuBar &mainMenuBar = MainMenuBar::GetInstance();
    if (!scene.IsLoaded()) {
        mainMenuBar.StopGamePlay();
        return;
    }

    if (mainMenuBar.IsGamePlaying() && !mainMenuBar.IsGamePaused()) {
        scene.Loop();
    } else {
        scene.PauseLoop();
    }
}

void GamePanel::ShowContents() {
    ImVec2 imgSize = ImGui::GetContentRegionAvail();
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowBgAlpha(0.35f);
    if (ImGui::Begin("Statistics", nullptr, windowFlags))
    {
        ImGui::Text("Statistics");
        ImGui::Text("Screen: %dx%d", int(imgSize.x), int(imgSize.y));
    }
    ImGui::End();

    Scene &scene = Scene::GetInstance();
    if (!scene.IsLoaded()) {
        ImGui::End();
        return;
    }
    
    gameFramebufferResource->SetWidth((int)imgSize.x);
    gameFramebufferResource->SetHeight((int)imgSize.y);

    ImGui::Image((void *)(intptr_t)gameFramebufferResource->GetColorTexture()->GetId(), 
        imgSize,
        ImVec2(0.0f, imgSize.y / gameFramebufferResource->GetMaxHeight()),
        ImVec2(imgSize.x / gameFramebufferResource->GetMaxWidth(), 0.0f));
}