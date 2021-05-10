#pragma once

#include <unordered_set>

#include <Graphics/Material.hpp>

class Mesh;
class Material;
class Drawer;
class Renderer;

class Batch {
private:
    Mesh *mesh;
    Material *material;
    std::unordered_set<Drawer *> drawers;

public:
    Batch(Mesh *mesh, Material *material) : mesh(mesh), material(material) {}
    virtual ~Batch();

    Mesh *GetMesh() const { return mesh; }
    Material *GetMaterial() const { return material; }
    
    void Refresh();
    void AddDrawer(Drawer *drawer);
    void RemoveDrawer(Drawer *drawer);
    void Draw(Renderer *renderer);
    
    friend class Drawer;
};