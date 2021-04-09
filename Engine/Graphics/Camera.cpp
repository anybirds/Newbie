#include <iostream>
#include <algorithm>
#include <glm/gtx/transform.hpp>

#include <GameObject.hpp>
#include <Transform.hpp>
#include <Scene.hpp>
#include <Project.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Material.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/Drawer.hpp>
#include <Graphics/Framebuffer.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Window.hpp>

using namespace glm;
using namespace std;
using namespace Engine;

Camera::Camera() : 
    dirty(true), orthographic(false), 
    fovy(60.0f), aspectRatio(1.0f), nr(0.1f), fr(1000.0f), size(5.0f) {}

const mat4 &Camera::GetNormalization() {
    if (dirty) {
        if (orthographic) {
            normalization = ortho(-size * aspectRatio, size * aspectRatio, -size, size, nr, fr);
        } else {
            normalization = perspective(radians(fovy), aspectRatio, nr, fr);
        }
        dirty = false;   
    }
    return normalization;
}

void Camera::Render() {
    int width, height;
    if (framebuffer) {
        width = framebuffer->GetWidth(); height = framebuffer->GetHeight();
        float framebufferAspectRatio = (float)width / height;
        if (aspectRatio != framebufferAspectRatio) {
            aspectRatio = framebufferAspectRatio;
            dirty = true;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
    } else {
        Project &project = Project::GetInstance();
        if (project.GetUseDefaultFramebuffer()) {
            Window &window = Window::GetInstance();
            width = window.GetFramebufferWidth(); height = window.GetFramebufferHeight();
            float windowAspectRatio = (float)width / height;
            if (aspectRatio != windowAspectRatio) {
                aspectRatio = windowAspectRatio;
                dirty = true;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        } else {
            width = project.GetGameFramebuffer()->GetWidth(); height = project.GetGameFramebuffer()->GetHeight();
            float framebufferAspectRatio = (float)width / height;
            if (aspectRatio != framebufferAspectRatio) {
                aspectRatio = framebufferAspectRatio;
                dirty = true;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, project.GetGameFramebuffer()->fbo);
        }
    }

    glViewport(0, 0, width, height);
    glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    for (Drawer *drawer : scene.drawers) {
        if (drawer->IsEnabled()) {
            drawer->Draw(this);
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
}

