#include <cassert>
#include <sstream>

#include <Class.hpp>
#include <Property.hpp>

using namespace std;
using namespace HeaderTool;

Class::Class(const std::wstring &macro) {
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
    base = args[1];
}

Class::~Class() {
    for (Property *property : properties) {
        delete property;
    }
}
wifstream &HeaderTool::operator>>(wifstream &ifs, Class &cs) {
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
                for (; !ifs.eof() && c != L'{' && c != L';' && str != L"public" && str != L"private" && str != L"protected"; ifs >> c) {
                    str += c;
                }
                assert(!ifs.eof());

                switch (c) {
                    case L'{':
                        do {
                            cnt = 0;
                            if (c == L'{') {
                                cnt++;
                            } else if (c == L'}') {
                                cnt--;
                            }
                            ifs >> c;
                        } while (cnt > 0);
                        break;
                    case L':':
                        break;
                    case L';':
                        if (str.substr(0, 8) == L"PROPERTY") {
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