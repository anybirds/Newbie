#include <cassert>
#include <sstream>

#include <Class.hpp>
#include <Property.hpp>

using namespace std;
using namespace HeaderTool;

Class::Class(const std::string &macro) : resource(false) {
    if (macro[5] == '_' && macro[6] == 'R') {
        resource = true;
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
    
    name = args[0];
    base = args[1];
}

Class::~Class() {
    for (Property *property : properties) {
        delete property;
    }
}

ifstream &HeaderTool::operator>>(ifstream &ifs, Class &cs) {
    while (!ifs.eof()) {
        char c;
        string str, temp;
        int cnt;

        ifs >> c;
        if (c == '}') {
            break;
        }
        switch (c) {
            case '#':
                do {
                    getline(ifs, temp);
                } while (temp.back() == '\\');
                break;
            default:
                for (; !ifs.eof() && c != '{' && c != ';' && str != "public" && str != "private" && str != "protected"; ifs >> c) {
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

                switch (c) {
                    case '{':
                        cnt = 1;
                        do {
                            c = ifs.get();
                            if (c == '{') {
                                cnt++;
                            } else if (c == '}') {
                                cnt--;
                            }
                        } while (!ifs.eof() && cnt > 0);
                        assert(!ifs.eof());
                        break;
                    case ':':
                        break;
                    case ';':
                        if (str.substr(0, 8) == "PROPERTY") {
                            Property *p = new Property(str);
                            cs.properties.push_back(p);
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