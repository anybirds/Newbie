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
