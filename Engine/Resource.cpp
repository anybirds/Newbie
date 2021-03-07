#include <cstdint>

#include <Resource.hpp>
#include <Project.hpp>
#include <Type.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

namespace Engine {
    void to_json(json &js, const shared_ptr<Resource> &resource) {
        if (resource) {
            js = reinterpret_cast<uint64_t>(resource.get());
        } else {
            js = nullptr;
        }
    }

    void from_json(json &js, shared_ptr<Resource> &resource) {
        uint64_t key = js.get<uint64_t>();
        if (key) {
            Project &project = Project::GetInstance();
            resource = project.GetAsset<Asset>(key)->GetResource();
        } else {
            resource = nullptr;
        }
    }
}

/*
#include <Common/Project.hpp>
#include <Common/Resource.hpp>

using namespace std;
using namespace Engine;

namespace Engine {
    TYPE_DEF(Resource)

    TOJSON_BEGIN(Resource)
    TOJSON_END

    FROMJSON_BEGIN(Resource)
    FROMJSON_END
}

bool Resource::sceneLoad;

Resource::Resource(const string &name, Type *type) : Object(name, type), loaded(false), shouldLoad(false) {
    Project::resset.insert(this);
}

Resource::~Resource() {
    Project::resset.erase(this);
}
*/