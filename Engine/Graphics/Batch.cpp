#include <iostream>

#include <Graphics/Batch.hpp>
#include <Graphics/Drawer.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Mesh.hpp>
#include <Transform.hpp>

using namespace std;
using namespace glm;

Batch::Batch(Mesh *mesh, Material *material) : mesh(mesh), material(material), transformBufferSize(1) {
    glGenBuffers(1, &transformBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, transformBuffer);
    glBufferData(GL_ARRAY_BUFFER, transformBufferSize * sizeof(mat4), NULL, GL_STATIC_DRAW);
}

Batch::~Batch() {
    glDeleteBuffers(1, &transformBuffer);
}

void Batch::AddDrawer(Drawer *drawer) {
    drawers.insert(drawer);
}

void Batch::RemoveDrawer(Drawer *drawer) {
    drawers.erase(drawer);
}

void Batch::Draw(Renderer *renderer, Material *material) {
    material = material ? material : this->material;
    if (!(mesh && material)) {
        throw exception();
    }

    // resize transform buffer
    GLsizeiptr newSize = 1;
    while (newSize < (GLsizeiptr)drawers.size()) {
        newSize <<= 1;
    }
    glBindBuffer(GL_ARRAY_BUFFER, transformBuffer);
    if (transformBufferSize < newSize) {
        transformBufferSize = newSize;
        glBufferData(GL_ARRAY_BUFFER, transformBufferSize * sizeof(mat4), NULL, GL_STATIC_DRAW); // also reallocate
    } else if (transformBufferSize >= (newSize << 2)) {
        transformBufferSize = (newSize << 1);
        glBufferData(GL_ARRAY_BUFFER, transformBufferSize * sizeof(mat4), NULL, GL_STATIC_DRAW);
    }

    // fill transform buffer
    GLsizeiptr i = 0;
    for (Drawer *drawer : drawers) {
        mat4 _MODEL = drawer->GetTransform()->GetLocalToWorldMatrix();
        glBufferSubData(GL_ARRAY_BUFFER, i++ * sizeof(mat4), sizeof(mat4), &_MODEL);
    }

    // GPU instanced array 
    glBindVertexArray(mesh->GetVertexArray());
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void *)(4 * sizeof(float)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void *)(8 * sizeof(float)));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void *)(12 * sizeof(float)));
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);

    // GPU instanced rendering
    material->UpdateUniforms(); // also use program

    GLuint location;
    location = glGetUniformLocation(material->GetProgram(), "_VIEW");
    mat4 _VIEW = renderer->GetTransform()->GetWorldToLocalMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&_VIEW);
    location = glGetUniformLocation(material->GetProgram(), "_NORM");
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&renderer->GetNormalization());

    if (glGetError() == GL_INVALID_OPERATION) {
        cerr << material->GetProgram() << '\n';
        cerr << "error before draw call\n";
    }

    if (mesh->GetElementCount()) {
        // mesh with EBO
        glDrawElementsInstanced(GL_TRIANGLES, mesh->GetElementCount(), GL_UNSIGNED_INT, 0, (GLsizei)drawers.size());
    }
    else {
        // mesh without EBO
        glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->GetVertexCount(), (GLsizei)drawers.size());
    }
    
    glBindVertexArray(0);
    glUseProgram(0);
}