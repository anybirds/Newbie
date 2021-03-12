#pragma once

#include <memory>
#include <string>

#include <Entity.hpp>

namespace Engine {

    class Resource;

    /*
    Menifest to how to instantiate the Resource object.
    */
    class ENGINE_EXPORT Asset : public Entity {
    private:
        std::string name;

    protected:
        std::weak_ptr<Resource> resource;

    public:
        virtual std::shared_ptr<Resource> GetResource() = 0;
        
        const std::string &GetName() const { return name; }
        void SetName(const std::string &name) { this->name = name; }
    };

    void to_json(nlohmann::json &js, const Asset *asset);
    void from_json(nlohmann::json &js, Asset *&asset);
}