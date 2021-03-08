#pragma once

#include <memory>

#include <EngineExport.hpp>
#include <Entity.hpp>

namespace Engine {

    class Resource;

    class ENGINE_EXPORT Asset : public Entity {
    private:
        uint64_t serial;
        std::weak_ptr<Resource> resource;

    public:
        virtual std::weak_ptr<Resource> &GetResource() = 0;

        friend class Project;
    };

    void to_json(nlohmann::json &js, const Asset *asset);
    void from_json(nlohmann::json &js, Asset *&asset);
}