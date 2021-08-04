#include <cassert>
#include <sstream>
#include <iostream>

#include <Property.hpp>

using namespace std;
using namespace HeaderTool;

Property::Property(const string &macro) {
    if (macro[8] == '_') {
        switch (macro[9]) {
            case 'N':
                mod = NONE;
                break;
            case 'G':
                mod = GET;
                break;
            case 'I':
                mod = IS;
                break;
            case 'S':
                mod = SET;
                break;
            case 'B':
                mod = BOOL;
                break;
            default:
                mod = DEFAULT;
                break;
        }
    } else {
        mod = DEFAULT;
    }

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
    memberName = args[1];
    propertyName = args[2];
}