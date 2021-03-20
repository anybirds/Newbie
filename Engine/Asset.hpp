#pragma once

#include <memory>
#include <string>

#include <Entity.hpp>
#include <Type.hpp>

namespace Engine {

    class Resource;

    /*
    Menifest to how to instantiate the Resource object.
    */
    class ENGINE_EXPORT [[Serialize]] Asset : public Entity {
        TYPE(Asset)
    private:
        std::string name;

    protected:
        [[NoSerialize]]
        std::weak_ptr<Resource> resource;

    public:
        virtual std::shared_ptr<Resource> GetResource() { return nullptr; };
        
        const std::string &GetName() const { return name; }
        void SetName(const std::string &name) { this->name = name; }
    };

    void to_json(nlohmann::json &js, const Asset *asset);
    void from_json(const nlohmann::json &js, Asset *&asset);
    
    template <typename T, std::enable_if_t<std::is_base_of_v<Entity, T>, bool> = true>
    void from_json(const nlohmann::json &js, T *&t) {
        if (std::is_base_of_v<Asset, T>) {
            Asset *asset = nullptr;
            from_json(js, asset);
            t = dynamic_cast<T *>(asset);
        } else {
            Entity *entity = nullptr;
            from_json(js, entity);
            t = dynamic_cast<T *>(entity);
        }
    }
}