#pragma once

#include <Component.hpp>

NAMESPACE(Engine) {
    
    class Camera;

    CLASS_ATTR(Drawer, Component, ENGINE_EXPORT) {
        TYPE(Drawer);
    
        PROPERTY_GET(unsigned, order, Order);
    
    public:
        Drawer() : order(0U) {}
        virtual void Draw(Camera *camera) = 0;

        void SetOrder(unsigned order);
    };
    
    class DrawerComparer {
    public:
        bool operator()(Drawer *l, Drawer *r) const {
            return l->GetOrder() < r->GetOrder();
        }
    };
}