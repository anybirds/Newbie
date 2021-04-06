#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace HeaderTool {
    
    class Property;

    class Class final {
    private:
        std::string name;
        std::string base;
        std::vector<std::string> args;
        std::vector<Property *> properties;

    public:
        Class(const std::wstring &macro);
        ~Class();
        friend std::wifstream &operator>>(std::wifstream &ifs, Class &cs);
        friend class Generator;
    };
}