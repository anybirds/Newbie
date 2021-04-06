#include <cassert>
#include <string>
#include <iostream>
#include <vector>

#include <Generator.hpp>

using namespace std;
using namespace HeaderTool;

int main(int argc, char **argv) {
    assert(argc == 2);
    
    string dir(argv[1]);

    Generator generator(dir);
    generator.Generate();   
    return 0;
}