#include <iostream>
#include <algorithm>

#include <GameObject.hpp>
#include <Transform.hpp>
#include <Scene.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Material.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/Batch.hpp>
#include <Graphics/Framebuffer.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Window.hpp>

using namespace glm;
using namespace std;
using namespace Engine;

void Camera::Render() {
    int width, height;
    const shared_ptr<Framebuffer> &framebuffer = GetFramebuffer();
    if (framebuffer) {
        width = framebuffer->GetWidth(); height = framebuffer->GetHeight();
        float framebufferAspectRatio = (float)width / height;
        if (GetAspectRatio() != framebufferAspectRatio) {
            SetAspectRatio(framebufferAspectRatio);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
    } else {
        Framebuffer *defaultFramebuffer = GetDefaultFramebuffer();
        if (defaultFramebuffer) {
            width = defaultFramebuffer->GetWidth(); height = defaultFramebuffer->GetHeight();
            float framebufferAspectRatio = (float)width / height;
            if (GetAspectRatio() != framebufferAspectRatio) {
                SetAspectRatio(framebufferAspectRatio);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer->fbo);
        } else {
            Window &window = Window::GetInstance();
            width = window.GetFramebufferWidth(); height = window.GetFramebufferHeight();
            float windowAspectRatio = (float)width / height;
            if (GetAspectRatio() != windowAspectRatio) {
                SetAspectRatio(windowAspectRatio);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    glViewport(0, 0, width, height);
    glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // render setting
    bool depth_test_enabled = glIsEnabled(GL_DEPTH_TEST);
    GLint depth_test_mode;
    glGetIntegerv(GL_DEPTH_FUNC, &depth_test_mode);

    bool cull_face_enabled = glIsEnabled(GL_CULL_FACE);
    GLint cull_face_mode;
    glGetIntegerv(GL_CULL_FACE_MODE, &cull_face_mode);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Scene &scene = Scene::GetInstance();
    for (auto &order : scene.GetAllBatches()) {
        for (auto &batch : order.second) {
            batch.second->Draw(this);
        }
    }

    // render setting
    if (depth_test_enabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    glDepthFunc(depth_test_mode);

    if (cull_face_enabled) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
    glCullFace(cull_face_mode);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (glGetError() != GL_NO_ERROR) {
        cerr << '[' << __FUNCTION__ << ']' << " graphics error in Camera: " << GetGameObject()->GetName() << '\n';
    }
}
