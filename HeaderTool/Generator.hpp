#pragma once

#include <string>
#include <vector>

namespace HeaderTool {

    class Namespace;

    class Generator final {
    private:
        std::vector<Namespace *> namespaces;
        std::vector<std::string> files;

        void Serialize();
        void Deserialize();
        void TypeInit();
        void TypeClear();
        
    public:
        Generator(const std::string &dir);
        ~Generator();

        void Generate();
    };
}