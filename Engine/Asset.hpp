#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <Entity.hpp>
#include <Type.hpp>
#include <Interface.hpp>

namespace Engine {

    class Resource;

    /*
    Menifest to how to instantiate the Resource object.
    */
    class ENGINE_EXPORT [[Serialize]] Asset : public Entity {
        TYPE(Asset)
    private:
        uint64_t serial;
        std::string name;

    protected:
        [[NoSerialize]]
        std::weak_ptr<Resource> resource;

    public:
        virtual std::shared_ptr<Resource> GetResource() = 0;
        
        const std::string &GetName() const { return name; }
        void SetName(const std::string &name) { this->name = name; }
        const uint64_t GetSerial() const { return serial; }

        friend class Project;
    };

    void ENGINE_EXPORT from_json(const nlohmann::json &js, Entity *&entity);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, Asset *&asset);

    template <typename T, std::enable_if_t<std::is_base_of_v<Entity, T> || std::is_base_of_v<Interface, T>, bool> = true>
    void to_json(nlohmann::json &js, const T *t) {
        if (std::is_base_of_v<Entity, T>) {
            js = reinterpret_cast<uint64_t>(t);
        } else {
            js = reinterpret_cast<uint64_t>(dynamic_cast<const Entity *>(t));
        }
    }   

    template <typename T, std::enable_if_t<std::is_base_of_v<Entity, T> || std::is_base_of_v<Interface, T>, bool> = true>
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