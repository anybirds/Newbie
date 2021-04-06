#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace HeaderTool {

    class Class;

    class Namespace final {
    private:
        std::string name;
        std::vector<std::string> args;
        std::vector<Namespace *> namespaces;
        std::vector<Class *> classes;

    public:
        Namespace(const std::wstring &macro);
        ~Namespace();
        friend std::wifstream &operator>>(std::wifstream &ifs, Namespace &ns);
        friend class Generator;
    };
}