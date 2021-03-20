#include <iostream>
#include <glm/gtx/transform.hpp>

#include <GameObject.hpp>
#include <Group.hpp>
#include <Transform.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Material.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/Renderer.hpp>

using namespace glm;
using namespace std;
using namespace Engine;

void Camera::ComputeNormalization() {
    if (orthographic) {
        normalization = ortho(left, right, bottom, top, nr, fr);
    } else {
        normalization = perspective(radians(fovy), width / height, nr, fr);
    }
}

void Camera::SetFovy(float fovy) {
    this->fovy = fovy;
    ComputeNormalization();
}

void Camera::SetWidth(float width) {
    this->width = width;
    ComputeNormalization();
}

void Camera::SetNear(float nr) {
    this->nr = nr;
    ComputeNormalization();
}

void Camera::SetFar(float fr) {
    this->fr = fr;
    ComputeNormalization();
}

void Camera::SetLeft(float left) {
    this->left = left;
    ComputeNormalization();
}

void Camera::SetRight(float right) {
    this->right = right;
    ComputeNormalization();
}
void Camera::SetTop(float top) {
    this->top = top;
    ComputeNormalization();
}

void Camera::SetBottom(float bottom) {
    this->bottom = bottom;
    ComputeNormalization();
}

void Camera::Render() {
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

    for (Component *idraw : GetGameObject()->GetGroup()->idraws) {
        if (!idraw->IsRemoved() && idraw->IsEnabled()) {
            ((IDraw *)idraw)->Draw(this);
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

