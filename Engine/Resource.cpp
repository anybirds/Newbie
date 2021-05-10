#include <cstdint>

#include <Resource.hpp>
#include <Project.hpp>
#include <Type.hpp>

using json = nlohmann::json;
using namespace std;

void to_json(json &js, const shared_ptr<Resource> &resource) {
    js = resource ? resource->GetSerial() : 0;
}

void from_json(const json &js, shared_ptr<Resource> &resource) {
    uint64_t key = js.get<uint64_t>();
    if (key) {
        Project &project = Project::GetInstance();
        resource = project.GetAsset<Asset>(key)->GetResource();
    } else {
        resource = nullptr;
    }
}

void Resource::Apply() {
    serial = asset->GetSerial();
    name = asset->GetName();
}