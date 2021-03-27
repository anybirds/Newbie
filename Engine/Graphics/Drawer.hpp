#pragma once

#include <Component.hpp>

namespace Engine {
    
    class Camera;

    class ENGINE_EXPORT [[Serialize]] Drawer : public Component {
        TYPE(Drawer)
    
    private:
        unsigned order;
    
    public:
        Drawer() : order(0U) {}
        virtual void Draw(Camera *camera) = 0;

        unsigned GetOrder() const { return order; }
        void SetOrder(unsigned order);
    };
    
    class DrawerComparer {
    public:
        bool operator()(Drawer *l, Drawer *r) const {
            return l->GetOrder() < r->GetOrder();
        }
    };
}