#include <cstdint>

#include <Resource.hpp>
#include <Project.hpp>
#include <Type.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

namespace Engine {
    void to_json(json &js, const shared_ptr<Resource> &resource) {
        js = resource->GetSerial();
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
}