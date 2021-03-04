#include <glm/gtx/transform.hpp>

#include <Common/Debug.hpp>
#include <Common/GameObject.hpp>
#include <Common/Scene.hpp>
#include <Common/Transform.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Material.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/Renderer.hpp>

using namespace glm;
using namespace std;
using namespace Engine;

namespace Engine {
    TYPE_DEF(Camera)
    SER_DEF(Camera, Component,
    MEMBER_SER | MEMBER_SHOW, bool, orthographic,
    MEMBER_SER | MEMBER_SHOW, float, fovy,
    MEMBER_SER | MEMBER_SHOW, float, width,
    MEMBER_SER | MEMBER_SHOW, float, height,
    MEMBER_SER | MEMBER_SHOW, float, near,
    MEMBER_SER | MEMBER_SHOW, float, far,
    MEMBER_SER | MEMBER_SHOW, float, left,
    MEMBER_SER | MEMBER_SHOW, float, right,
    MEMBER_SER | MEMBER_SHOW, float, bottom,
    MEMBER_SER | MEMBER_SHOW, float, top
    )
}

unordered_set<Renderer *> Camera::rendset;

Camera *Camera::GetMainCamera() {
    return Find<SceneSetting>("RenderSetting")->GetMainCamera();
}

void Camera::SetMainCamera(Camera *camera) {
    Find<SceneSetting>("RenderSetting")->SetMainCamera(camera);
}

Camera::Camera(const std::string &name, Type *type) : Component(name, type) {

}

Camera::~Camera() {

}

void Camera::ComputeNormalization() {
    if (orthographic) {
        normalization = ortho(left, right, bottom, top, near, far);
    } else {
        normalization = perspective(radians(fovy), width / height, near, far);
    }
}

void Camera::OnInit() {
    ComputeNormalization();
}

void Camera::SetFovy(float fovy) {
    this->fovy = fovy;
    ComputeNormalization();
}
void Camera::SetWidth(float width) {
    this->width = width;
    ComputeNormalization();
}

void Camera::SetNear(float near) {
    this->near = near;
    ComputeNormalization();
}

void Camera::SetFar(float far) {
    this->far = far;
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

    for (Renderer *rend : rendset) {
        Mesh *mesh = rend->GetMesh();
        Material *material = rend->GetMaterial();

        glBindVertexArray(mesh->vao);

        glUseProgram(material->program);
        material->UseTextures();

        GLuint location;

        location = glGetUniformLocation(material->program, "_MODEL");

        mat4 _MODEL = rend->GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&_MODEL);

        location = glGetUniformLocation(material->program, "_CAM");
        mat4 _CAM = GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&_CAM);
        location = glGetUniformLocation(material->program, "_NORM");
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&normalization);

        if (!mesh->icnt) {
            // mesh without EBO
            glDrawArrays(GL_TRIANGLES, 0, mesh->vcnt);
        }
        else {
            // mesh with EBO
            glDrawElements(GL_TRIANGLES, mesh->icnt, GL_UNSIGNED_INT, 0);
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

