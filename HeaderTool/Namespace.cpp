#include <cassert>
#include <sstream>

#include <Class.hpp>
#include <Namespace.hpp>

using namespace std;
using namespace HeaderTool;

Namespace::Namespace(const wstring &macro) {
    // global namespace
    if (macro.empty()) {
        return;
    }
    
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

wifstream &HeaderTool::operator>>(wifstream &ifs, Namespace &ns) {
    while (!ifs.eof()) {
        wchar_t c;
        wstring str;
        int cnt;

        ifs >> c;
        if (c == L'}') {
            break;
        }
        switch (c) {
            case L'#':
                do {
                    getline(ifs, str);
                } while (str.back() == L'\\');
                break;
            case L'/':
                c = ifs.get();
                switch (c) {
                    case L'/':
                        getline(ifs, str);
                        break;
                    case L'*':
                        do {
                            ifs >> c;
                            if (c == L'*') {
                                c = ifs.get();
                                if (c == L'/') {
                                    break;
                                }
                            }
                        } while (!ifs.eof());
                        assert(!ifs.eof());
                        break;
                    default:
                        assert(false);
                        break;
                }
                break;
            default:
                for (; !ifs.eof() && c != L'{' && c != L';'; c = ifs.get()) {
                    str += c;
                }
                assert(!ifs.eof());

                switch (c) {
                    case L'{':
                        if (str.substr(0, 9) == L"NAMESPACE") {
                            Namespace *n = new Namespace(str);
                            ns.namespaces.push_back(n);
                            ifs >> *n;
                        } else if (str.substr(0, 5) == L"CLASS") {
                            Class *cs = new Class(str);
                            ns.classes.push_back(cs);
                            ifs >> *cs;
                        } else {
                            cnt = 1;
                            do {
                                c = ifs.get();
                                if (c == L'{') {
                                    cnt++;
                                } else if (c == L'}') {
                                    cnt--;
                                }
                            } while (!ifs.eof() && cnt > 0);
                            assert(!ifs.eof());
                        } 
                        break;
                    case L';':
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