#pragma once

#include <memory>

#include <Entity.hpp>

namespace Engine {

    class Resource;

    /*
    Menifest to how to instantiate the Resource object.
    */
    class ENGINE_EXPORT Asset : public Entity {
    private:
        uint64_t serial;

    protected:
        std::weak_ptr<Resource> resource;

    public:
        virtual std::shared_ptr<Resource> GetResource() = 0;

        friend class Project;
    };

    void to_json(nlohmann::json &js, const Asset *asset);
    void from_json(nlohmann::json &js, Asset *&asset);
}