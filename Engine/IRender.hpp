#pragma once

#include <EngineExport.hpp>
#include <Interface.hpp>

namespace Engine {
    
    class ENGINE_EXPORT IRender : public Interface {
    private:
        unsigned order;
        
    public:
        IRender() : order(0U) {}
        virtual void Render() = 0;

        unsigned GetOrder() const { return order; }
        void SetOrder(unsigned order) { this->order = order; }
    };

    class IRenderCompare {
    public:
        bool operator()(IRender *l, IRender *r) {
            return l->GetOrder() < r->GetOrder();
        }
    };
    
}