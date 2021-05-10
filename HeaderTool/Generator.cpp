#include <functional>
#include <filesystem>

#include <Generator.hpp>
#include <Namespace.hpp>
#include <Class.hpp>
#include <Property.hpp>

using namespace std;
using namespace HeaderTool;

Generator::Generator(const string &dir) {
    function<void(const std::string &)> parse = [&parse, this](const std::string &path) {
        for (auto &p : std::filesystem::directory_iterator(filesystem::u8path(path))) {
            if (p.is_directory()) {
                parse(p.path().u8string());
            } else if (p.path().extension() == ".hpp" || p.path().extension() == ".h") {

                cerr << p.path().filename().string() << '\n';

                ifstream ifs(p.path());
                if (ifs.fail()) {
                    cerr << '[' << __FUNCTION__ << ']' << " cannot open file: " << p.path().filename().string() << '\n';
                    throw exception();
                }

                files.push_back(p.path().u8string());

                Namespace *ns = new Namespace("");
                namespaces.push_back(ns);
                ifs >> *ns;
            }
        }
    };
    parse(dir);
}

Generator::~Generator() {
    for (Namespace *ns : namespaces) {
        delete ns;
    }
}

void Generator::Serialize() {
    function<void(Namespace *ns)> write = [&write](Namespace *ns) {
        for (Namespace *n : ns->namespaces) {
            cout << "namespace " << n->name << " {\n";
            write(n);
            cout << "}\n";
        }
        for (Class *cs : ns->classes) {
            cout << "void " << cs->name << "::Serialize(json &js, const Entity *entity) {\n";
            
            if (!cs->base.empty()) {
                cout << "  " << cs->base << "::StaticType()->Serialize(js, entity);\n";
            }

            cout << "  const " << cs->name << " *e = (const " << cs->name << " *)entity;\n";
            for (Property *p : cs->properties) {
                cout << "  js.push_back(e->" << p->name << ");\n";
            }

            cout << "}\n";
        }
    };
    for (Namespace *ns : namespaces) {
        write(ns);
    }
}

void Generator::Deserialize() {
    function<void(Namespace *ns)> write = [&write](Namespace *ns) {
        for (Namespace *n : ns->namespaces) {
            cout << "namespace " << n->name << " {\n";
            write(n);
            cout << "}\n";
        }
        for (Class *cs : ns->classes) {
            cout << "size_t " << cs->name << "::Deserialize(const json &js, Entity *entity) {\n";

            cout << "  size_t i = ";
            if (cs->base.empty()) {
                cout << "0;\n";
            } else {
                cout << cs->base << "::StaticType()->Deserialize(js, entity);\n";
            }
        
            cout << "  " << cs->name << " *e = (" << cs->name << " *)entity;\n";
            for (size_t i = 0; i < cs->properties.size(); i++) {
                Property *p = cs->properties[i];
                cout << "  e->" << p->name << " = js[i++].get<" << p->type << ">();\n";
            }

            cout << "  return i;\n";

            cout << "}\n";
        }
    };
    for (Namespace *ns : namespaces) {
        write(ns);
    }
}

void Generator::TypeInit() {
    vector<Namespace *> stack;
    function<void(Namespace *ns)> write = [&write, &stack](Namespace *ns) {
        for (Namespace *n : ns->namespaces) {
            stack.push_back(n);
            write(n);
            stack.pop_back();
        }
        for (Class *cs : ns->classes) {
            string name;
            for (Namespace *n : stack) {
                name += n->name;
                name += "::";
            }
            name += cs->name;
            cout << "  " << name << "::SetType(new Type(\"" << name << "\"));\n";
            cout << "  " << name << "::StaticType()->SetInstantiate(" << "instantiate<" << name << ", true>);\n";
            cout << "  " << name << "::StaticType()->SetSerialize(" << name << "::Serialize);\n";
            cout << "  " << name << "::StaticType()->SetDeserialize(" << name << "::Deserialize);\n";
        }
    };

    cout << "void type_init() {\n";
    for (Namespace *ns : namespaces) {
        write(ns);
    }
    cout << "}\n";
}

void Generator::TypeClear() {
    vector<Namespace *> stack;
    function<void(Namespace *ns)> write = [&write, &stack](Namespace *ns) {
        for (Namespace *n : ns->namespaces) {
            stack.push_back(n);
            write(n);
            stack.pop_back();
        }
        for (Class *cs : ns->classes) {
            string name;
            for (Namespace *n : stack) {
                name += n->name;
                name += "::";
            }
            name += cs->name;
            cout << "  delete " << name << "::StaticType();\n";
        }
    };

    cout << "void type_clear() {\n";
    for (Namespace *ns : namespaces) {
        write(ns);
    }
    cout << "}\n";
}

void Generator::Generate() {
    for (auto &file : files) {
        cout << "#include \"" << file << "\"\n";
    }
    
    cout << "#include <nlohmann/json.hpp>\n";
    cout << "using json = nlohmann::json;\n";

    Serialize();
    Deserialize();
    TypeInit();
    TypeClear();
}