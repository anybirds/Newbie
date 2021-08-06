#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace HeaderTool {

    class Class;
    class Enum;

    class Namespace final {
    private:
        std::string name;
        std::vector<std::string> args;
        std::vector<Namespace *> namespaces;
        std::vector<Class *> classes;
        std::vector<Enum *> enums;

    public:
        Namespace(const std::string &macro);
        ~Namespace();
        friend std::ifstream &operator>>(std::ifstream &ifs, Namespace &ns);
        friend class Generator;
    };
}