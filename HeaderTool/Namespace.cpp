#include <cassert>
#include <sstream>

#include <Class.hpp>
#include <Namespace.hpp>

using namespace std;
using namespace HeaderTool;

Namespace::Namespace(const string &macro) {
    // global namespace
    if (macro.empty()) {
        return;
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
}

Namespace::~Namespace() {
    for (Class *cs : classes) {
        delete cs;
    }
    for (Namespace *ns : namespaces) {
        delete ns;
    }
}

ifstream &HeaderTool::operator>>(ifstream &ifs, Namespace &ns) {
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
                for (; !ifs.eof() && c != '{' && c != ';'; ifs >> c) {
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
                        if (str.substr(0, 9) == "NAMESPACE") {
                            Namespace *n = new Namespace(str);
                            ns.namespaces.push_back(n);
                            ifs >> *n;
                        } else if (str.substr(0, 5) == "CLASS") {
                            Class *cs = new Class(str);
                            ns.classes.push_back(cs);
                            ifs >> *cs;
                        } else {
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
                        } 
                        break;
                    case ';':
                        break;
                    default:
                        assert(false);
                        break;
                }
                break;
        }
    }

    return ifs;
}