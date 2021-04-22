#pragma once

#include <unordered_set>

#include <Entity.hpp>
#include <Graphics/Material.hpp>

namespace Engine {

    class Mesh;
    class Material;
    class Drawer;

    struct BatchKey {
        Mesh *mesh;
        Material *material;
    };
    
    class Batch : public Entity {
    private:
        BatchKey key;
        std::unordered_set<Drawer *> drawers;
    
    public:
        std::unordered_set<Drawer *> &GetDrawers() { return drawers; }
        void Refresh();
        void Add(Drawer *drawer);
        void Remove(Drawer *drawer);
    };

    class BatchComparer {
        bool operator()(const BatchKey &l, const BatchKey &r) {
            return l.material->GetOrder() < r.material->GetOrder();
        }
    };
}