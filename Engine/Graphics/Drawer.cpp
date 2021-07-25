#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Transform.hpp>
#include <Scene.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Drawer.hpp>
#include <Graphics/Mesh.hpp>
#include <Graphics/Material.hpp>
#include <Graphics/Batch.hpp>

using namespace std;
using namespace glm;

void Drawer::Draw(Renderer *renderer, std::shared_ptr<Material> material) {
    material = material ? material : this->material;
    if (!(mesh && material)) {
        return;
    }

    glBindVertexArray(mesh->GetVertexArray());
    material->UseProgramAndTextures();

    GLuint location;
    location = glGetUniformLocation(material->GetProgram(), "_MODEL");
    mat4 _MODEL = GetTransform()->GetLocalToWorldMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&_MODEL);
    location = glGetUniformLocation(material->GetProgram(), "_VIEW");
    mat4 _VIEW = renderer->GetTransform()->GetWorldToLocalMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&_VIEW);
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

    glBindVertexArray(0);
    glUseProgram(0);
}

void Drawer::OnTrack() {
    if (!material) {
        return;
    }

    Scene &scene = Scene::GetInstance();
    Batch *batch;
    auto key = make_pair(mesh.get(), material.get());
    auto &batchmap = scene.batches[material->GetOrder()];
    auto it = batchmap.find(key);
    if (it == batchmap.end()) {
        batch = new Batch(key.first, key.second);
        batchmap.insert(make_pair(key, batch));
    } else {
        batch = it->second;
    }
    batch->AddDrawer(this);
}

void Drawer::OnUntrack() {
    if (!material) {
        return;
    }

    Scene &scene = Scene::GetInstance();
    auto i = scene.batches.find(material->GetOrder());
    auto j = i->second.find(make_pair(mesh.get(), material.get()));
    j->second->RemoveDrawer(this);
    if (j->second->drawers.empty()) {
        delete j->second;
        i->second.erase(j);
        if (i->second.empty()) {
            scene.batches.erase(i);
        }
    }
}

void Drawer::SetMesh(const shared_ptr<Mesh> &mesh) {
    if (mesh == this->mesh) {
        return;
    }

    Scene &scene = Scene::GetInstance();
    auto i = scene.batches.find(material->GetOrder());
    if (i != scene.batches.end()) {
        auto j = i->second.find(make_pair(this->mesh.get(), material.get()));
        if (j != i->second.end()) {
            j->second->RemoveDrawer(this);
            if (j->second->drawers.empty()) {
                delete j->second;
                i->second.erase(j);
            }
            Batch *&batch = i->second[make_pair(mesh.get(), material.get())];
            if (!batch) {
                batch = new Batch(mesh.get(), material.get());
            }
            batch->AddDrawer(this);
        }
    }
    this->mesh = mesh;
}

void Drawer::SetMaterial(const shared_ptr<Material> &material) {
    if (material == this->material) {
        return;
    }

    unsigned prevOrder = this->material ? this->material->GetOrder() : 0;
    unsigned nextOrder = material ? material->GetOrder() : 0;

    Scene &scene = Scene::GetInstance();
    auto i = scene.batches.find(prevOrder);
    if (i != scene.batches.end()) {
        auto j = i->second.find(make_pair(mesh.get(), this->material.get()));
        if (j != i->second.end()) {
            j->second->RemoveDrawer(this);
            if (j->second->drawers.empty()) {
                delete j->second;
                i->second.erase(j);
                if (i->second.empty()) {
                    scene.batches.erase(i);
                }
            }
            Batch *&batch = scene.batches[nextOrder][make_pair(mesh.get(), material.get())];
            if (!batch) {
                batch = new Batch(mesh.get(), material.get());
            }
            batch->AddDrawer(this);
        }
    }
    this->material = material;
}