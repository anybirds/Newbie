#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace HeaderTool {
    class Enum final {
    private:
        std::string name;
        std::string base;
        std::vector<std::string> args;
        std::vector<std::pair<std::string, std::string>> enums;

    public:
        Enum(const std::string &macro);
        friend std::ifstream &operator>>(std::ifstream &ifs, Enum &e);
        friend class Generator;
    };
}