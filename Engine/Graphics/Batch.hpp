#pragma once

#include <unordered_set>

#include <Graphics/Material.hpp>

class Mesh;
class Material;
class Drawer;
class Renderer;

class ENGINE_EXPORT Batch {
private:
    Mesh *mesh;
    Material *material;
    std::unordered_set<Drawer *> drawers;

    GLuint transformBuffer;
    GLsizeiptr transformBufferSize;

public:
    Batch(Mesh *mesh, Material *material);
    virtual ~Batch();

    Mesh *GetMesh() const { return mesh; }
    Material *GetMaterial() const { return material; }
    const std::unordered_set<Drawer *> &GetDrawers() const { return drawers; }

    void AddDrawer(Drawer *drawer);
    void RemoveDrawer(Drawer *drawer);
    void Draw(Renderer *renderer, Material *material = nullptr);

    friend class Drawer;
};