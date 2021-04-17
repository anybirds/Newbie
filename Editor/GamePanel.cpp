#include <Engine.hpp>
#include <GamePanel.hpp>

using namespace std;
using namespace Engine;

GamePanel::GamePanel() : Panel("Game") {
    Window &window = Window::GetInstance();

    gameTexture = new ATexture();
    gameTexture->SetWidth(window.GetMonitorWidth());
    gameTexture->SetHeight(window.GetMonitorHeight());

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
    if (!scene.IsLoaded()) {
        running = false;
        paused = false;
        return;
    }

    if (running) {
        Time::Tick();

        if (paused) {
            scene.Render();
        } else {
            scene.Refresh();
            scene.Update();
            scene.Render();

            scene.Delete();
        }
    } else {
        scene.Render();
    }
}

void GamePanel::ShowContents() {
    Scene &scene = Scene::GetInstance();
    if (!scene.IsLoaded()) {
        ImGui::End();
        return;
    }

    Project &project = Project::GetInstance();
    // todo: set game framebuffer width, height to reference width, height

    ImVec2 imgSize = ImGui::GetContentRegionAvail();

    // todo: do fancy uv stuff to preserve aspect ratio
    ImGui::Image((void *)(intptr_t)gameFramebufferResource->GetColorTexture()->GetId(), 
    imgSize,
    ImVec2(0.0f, 1.0f),
    ImVec2(1.0f, 0.0f));
}

void GamePanel::ShowPlayPause() {
    Scene &scene = Scene::GetInstance();
    ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
    if (ImGui::Selectable(ICON_FA_PLAY, running, 0, ImVec2(16.0f, 0.0f)) && scene.IsLoaded()) {
        if (running) {
            scene.Load(scene.GetPath());
        } else {
            scene.Save();
            Time::Init();
            scene.Start();
        }
        running ^= true;
        paused = false;
    }
    if (ImGui::Selectable(ICON_FA_PAUSE, paused, 0, ImVec2(16.0f, 0.0f)) && scene.IsLoaded()) {
        paused ^= true;
    }
    ImGui::PopStyleColor();
}