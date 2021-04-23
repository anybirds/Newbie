#pragma once

#include <unordered_set>

#include <Graphics/Material.hpp>

namespace Engine {

    class Mesh;
    class Material;
    class Drawer;
    class Renderer;
    
    struct BatchKey {
        Mesh *mesh;
        Material *material;
    };
    
    class Batch {
    private:
        BatchKey key;
        std::unordered_set<Drawer *> drawers;
    
    public:
        Batch(const BatchKey &key) : key(key) {}
        virtual ~Batch();

        const BatchKey &GetKey() const { return key; }
        void Refresh();
        void AddDrawer(Drawer *drawer);
        void RemoveDrawer(Drawer *drawer);
        void Draw(Renderer *renderer);
    };
}