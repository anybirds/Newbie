#include <cassert>
#include <sstream>

#include <Property.hpp>

using namespace std;
using namespace HeaderTool;

Property::Property(const wstring &macro) {
    wstringstream ss;
    ss << macro;
    wchar_t c;
    do {
        ss >> c;
    }
    while (!ss.eof() && c != L'(');
    assert(!ss.eof());

    do {
        wstring arg;
        ss >> c;
        while (!ss.eof() && c != L',' && c != L')') {
            arg += c;
            c = ss.get();
        }
        args.push_back(string(arg.begin(), arg.end()));
    } while (!ss.eof() && c != L')');
    assert(!ss.eof());
    
    type = args[0];
    name = args[1];
}