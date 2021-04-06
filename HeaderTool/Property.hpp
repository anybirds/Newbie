#pragma once

#include <string>
#include <vector>

namespace HeaderTool {
    class Property final {
    private:
        std::string type;
        std::string name;
        std::vector<std::string> args;

    public:
        Property(const std::wstring &macro);
        friend class Generator;
    };
}