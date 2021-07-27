#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>

#include <Entity.hpp>
#include <Type.hpp>

#include <iostream>

class Resource;

CLASS_ATTR(Asset, Entity, ENGINE_EXPORT) {
    TYPE(Asset);

    PROPERTY_GET(uint64_t, serial, Serial);
    PROPERTY(std::string, name, Name);

protected:
    std::weak_ptr<Resource> resource;

public:
    void Apply();
    void Remove();
    bool IsRemoved();
    bool IsResourced() { return !resource.expired(); }
    virtual std::shared_ptr<Resource> GetResource() = 0;
    
    friend class Project;
};

void ENGINE_EXPORT from_json(const nlohmann::json &js, Entity *&entity);
void ENGINE_EXPORT from_json(const nlohmann::json &js, Asset *&asset);

template <typename T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool> = true>
void to_json(nlohmann::json &js, const T *t) {
    js = t->GetSerial();
}   
template <typename T, std::enable_if_t<std::is_base_of_v<Entity, T> &&
!std::is_base_of_v<Asset, T>, bool> = true>
void to_json(nlohmann::json &js, const T *t) {
    js = (uint64_t)t;
}

template <typename T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool> = true>
void from_json(const nlohmann::json &js, T *&t) {
    Asset *asset = nullptr;
    from_json(js, asset);
    t = (T *)asset;
}
template <typename T, std::enable_if_t<std::is_base_of_v<Entity, T> &&
!std::is_base_of_v<Asset, T>, bool> = true> 
void from_json(const nlohmann::json &js, T *&t) {
    Entity *entity = nullptr;
    from_json(js, entity);
    t = (T *)entity;
}