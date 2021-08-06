#include <cassert>
#include <sstream>

#include <Enum.hpp>

using namespace std;
using namespace HeaderTool;

Enum::Enum(const std::string &macro) {
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
    
    name = args[0];
    base = args[1];
}

ifstream &HeaderTool::operator>>(ifstream &ifs, Enum &e) {
    while (!ifs.eof()) {
        char c;
        string str, temp;

        ifs >> c;
        if (c == ';') {
            break;
        }
        switch (c) {
            case '#':
                do {
                    getline(ifs, temp);
                } while (temp.back() == '\\');
                break;
            default:
                for (; !ifs.eof() && c != '}' && c != ','; ifs >> c) {
                    switch (c) {
                        case '/':
                            c = ifs.get();
                            switch (c) {
                                case '/':
                                    getline(ifs, temp);
                                    break;
                                case '*':
                                    c = ifs.get();
                                    do {
                                        temp += c;
                                        c = ifs.get();
                                    } while (!ifs.eof() && !(c == '/' && temp.back() == '*'));
                                    assert(!ifs.eof());
                                    break;
                                default:
                                    assert(false);
                                    break;
                            }
                            break;
                        case '\'':
                            do {
                                str += c;
                                c = ifs.get();
                            } while (!ifs.eof() && !(c == '\'' && str.back() != '\\'));
                            assert(!ifs.eof());
                            break;
                        case '\"':
                            do {
                                str += c;
                                c = ifs.get();
                            } while (!ifs.eof() && !(c == '\"' && str.back() != '\\'));
                            assert(!ifs.eof());
                            break;
                        default:
                            str += c;
                            break;
                    }
                }
                assert(!ifs.eof());

                size_t offset = str.find('=');
                switch (c) {
                    case ',':
                    case '}':
                        if (!str.empty()) {
                            e.enums.push_back({str.substr(0, offset), str.substr(offset + 1)});
                        }
                        break;
                    default:
                        assert(false);
                        break;
                }
        } 
    }

    return ifs;
}