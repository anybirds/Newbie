#include <cstdint>

#include <Asset.hpp>
#include <Project.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

namespace Engine {
    void to_json(json &js, const Asset *asset) {
        if (asset) {
            js = reinterpret_cast<uint64_t>(asset);
        } else {
            js = nullptr;
        }
    }

    void from_json(const json &js, Asset *&asset) {
        uint64_t key = js.get<uint64_t>();
        if (key) {
            Project &project = Project::GetInstance();
            asset = reinterpret_cast<Asset *>(project.GetAsset<Asset>(key));
        } else {
            asset = nullptr;
        }
    }
}
