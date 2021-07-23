#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace HeaderTool {
    class Property final {
    private:
        uint8_t mod;
        std::string type;
        std::string memberName;
        std::string propertyName;
        std::vector<std::string> args;

    public:
        enum : uint8_t {
            DEFAULT,
            BOOL,
            GET,
            IS,
            SET,
            NONE
        };
        Property(const std::string &macro);
        friend class Generator;
    };
}