#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <GameObject.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/Material.hpp>

using namespace std;
using namespace glm;
using namespace Engine;

void Renderer::Draw(Camera *camera) {
    if (!mesh) {
        cerr << '[' << __FUNCTION__ << ']' << " missing Mesh in GameObject:" << GetGameObject()->GetName() << '\n';
        throw exception();
    }
    if (!material) {
        cerr << '[' << __FUNCTION__ << ']' << " missing Material in GameObject:" << GetGameObject()->GetName() << '\n';
        throw exception();
    }
    glBindVertexArray(mesh->vao);

    glUseProgram(material->program);
    material->UseTextures();

    GLuint location;

    location = glGetUniformLocation(material->program, "_MODEL");

    mat4 _MODEL = GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            cerr << _MODEL[i][j] << ' ';
        }
        cerr << '\n';
    }
    cerr << '\n';
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&_MODEL);

    location = glGetUniformLocation(material->program, "_CAM");
    mat4 _CAM = camera->GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            cerr << _CAM[i][j] << ' ';
        }
        cerr << '\n';
    }
    cerr << '\n';
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&_CAM);
    location = glGetUniformLocation(material->program, "_NORM");
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&camera->GetNormalization());

    if (!mesh->icnt) {
        // mesh without EBO
        glDrawArrays(GL_TRIANGLES, 0, mesh->vcnt);
    }
    else {
        // mesh with EBO
        glDrawElements(GL_TRIANGLES, mesh->icnt, GL_UNSIGNED_INT, 0);
    }   
}