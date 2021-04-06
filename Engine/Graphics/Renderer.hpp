#pragma once

#include <Component.hpp>

NAMESPACE(Engine) {
    CLASS_ATTR(Renderer, Component, ENGINE_EXPORT) {
        TYPE(Renderer);
    
        PROPERTY_GET(unsigned, order, Order);
    
    public:
        Renderer() : order(0U) {}
        virtual void Render() = 0;

        void SetOrder(unsigned order);
    };
    
    class RendererComparer {
    public:
        bool operator()(Renderer *l, Renderer *r) const {
            return l->GetOrder() < r->GetOrder();
        }
    };
}