#include <cassert>
#include <string>
#include <filesystem>

#include <Generator.hpp>

using namespace std;
using namespace HeaderTool;

int main(int argc, char **argv) {
    assert(argc == 2);
    
#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    string dir(filesystem::path(argv[1]).u8string());
#else
    string dir(argv[1]);
#endif

    Generator generator(dir);
    generator.Generate();   
    return 0;
}