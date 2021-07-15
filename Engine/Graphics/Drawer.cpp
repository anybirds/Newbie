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
        i->second.erase(j);
        delete j->second;
        if (i->second.empty()) {
            scene.batches.erase(i);
        }
    }
}

void Drawer::SetMesh(const shared_ptr<Mesh> &mesh) {
    Scene &scene = Scene::GetInstance();
    auto i = scene.batches.find(material->GetOrder());
    if (i != scene.batches.end()) {
        auto j = i->second.find(make_pair(this->mesh.get(), material.get()));
        if (j != i->second.end()) {
            j->second->RemoveDrawer(this);
            if (j->second->drawers.empty()) {
                i->second.erase(j);
                delete j->second;
                if (i->second.empty()) {
                    scene.batches.erase(i);
                }
            }
            i->second[make_pair(mesh.get(), material.get())]->AddDrawer(this);
        }
    }
    this->mesh = mesh;
}

void Drawer::SetMaterial(const shared_ptr<Material> &material) {
    Scene &scene = Scene::GetInstance();
    auto i = scene.batches.find(this->material->GetOrder());
    if (i != scene.batches.end()) {
        auto j = i->second.find(make_pair(mesh.get(), this->material.get()));
        if (j != i->second.end()) {
            j->second->RemoveDrawer(this);
            if (j->second->drawers.empty()) {
                i->second.erase(j);
                delete j->second;
                if (i->second.empty()) {
                    scene.batches.erase(i);
                }
            }
            scene.batches[material->GetOrder()][make_pair(mesh.get(), material.get())]->AddDrawer(this);
        }
    }
    this->material = material;
}