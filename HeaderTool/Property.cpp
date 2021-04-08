#include <cassert>
#include <sstream>

#include <Property.hpp>

using namespace std;
using namespace HeaderTool;

Property::Property(const string &macro) {
    stringstream ss;
    ss << macro;
    char c;
    do {
        ss >> c;
    }
    while (!ss.eof() && c != '(');
    assert(!ss.eof());

    do {
        string arg;
        ss >> c;
        while (!ss.eof() && c != ',' && c != ')') {
            arg += c;
            c = ss.get();
        }
        args.push_back(arg);
    } while (!ss.eof() && c != ')');
    assert(!ss.eof());
    
    type = args[0];
    name = args[1];
}