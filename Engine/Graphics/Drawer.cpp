#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <GameObject.hpp>
#include <Scene.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Drawer.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/Material.hpp>
#include <Graphics/Batch.hpp>

using namespace std;
using namespace glm;
using namespace Engine;

void Drawer::Draw(Renderer *renderer) {
    if (!mesh) {
        cerr << '[' << __FUNCTION__ << ']' << " missing Mesh in GameObject: " << GetGameObject()->GetName() << '\n';
        throw exception();
    }
    if (!material) {
        cerr << '[' << __FUNCTION__ << ']' << " missing Material in GameObject: " << GetGameObject()->GetName() << '\n';
        throw exception();
    }

    glBindVertexArray(mesh->GetVertexArray());

    glUseProgram(material->GetProgram());

    material->UseTextures();

    GLuint location;

    location = glGetUniformLocation(material->GetProgram(), "_MODEL");

    mat4 _MODEL = GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&_MODEL);

    location = glGetUniformLocation(material->GetProgram(), "_CAM");
    mat4 _CAM = renderer->GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&_CAM);
    location = glGetUniformLocation(material->GetProgram(), "_NORM");
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&renderer->GetNormalization());

    if (!mesh->GetElementCount()) {
        // mesh without EBO
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertexCount());
    }
    else {
        // mesh with EBO
        glDrawElements(GL_TRIANGLES, mesh->GetElementCount(), GL_UNSIGNED_INT, 0);
    }   

    glUseProgram(0);
}

void Drawer::OnEnable() {
    Scene &scene = Scene::GetInstance();
    Batch *batch;
    BatchKey key{mesh.get(), material.get()};
    auto &batchmap = scene.batches[material->GetOrder()];
    auto it = batchmap.find(key);
    if (it == batchmap.end()) {
        batch = new Batch(key);
        batchmap.insert(make_pair(key, batch));
    } else {
        batch = it->second;
    }
    batch->AddDrawer(this);
}

void Drawer::OnDisable() {
    Scene &scene = Scene::GetInstance();
    scene.batches[material->GetOrder()][BatchKey{mesh.get(), material.get()}]->RemoveDrawer(this);
}

void Drawer::SetMesh(const shared_ptr<Mesh> &mesh) {
    Scene &scene = Scene::GetInstance();
    auto i = scene.batches.find(material->GetOrder());
    if (i != scene.batches.end()) {
        auto j = i->second.find(BatchKey{this->mesh.get(), material.get()});
        if (j != i->second.end()) {
            j->second->RemoveDrawer(this);
            i->second[BatchKey{mesh.get(), material.get()}]->AddDrawer(this);
        }
    }
    this->mesh = mesh;
}

void Drawer::SetMaterial(const shared_ptr<Material> &material) {
    Scene &scene = Scene::GetInstance();
    auto i = scene.batches.find(this->material->GetOrder());
    if (i != scene.batches.end()) {
        auto j = i->second.find(BatchKey{mesh.get(), this->material.get()});
        if (j != i->second.end()) {
            j->second->RemoveDrawer(this);
            scene.batches[material->GetOrder()][BatchKey{mesh.get(), material.get()}]->AddDrawer(this);
        }
    }
    this->material = material;
}