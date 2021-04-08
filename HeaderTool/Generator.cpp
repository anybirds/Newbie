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
        for (auto &p : std::filesystem::directory_iterator(path)) {
            if (p.is_directory()) {
                parse(p.path().string());
            } else if (p.path().extension() == ".hpp" || p.path().extension() == ".h") {

                cerr << p.path().string() << '\n';

                ifstream ifs(p.path().string());
                if (ifs.fail()) {
                    cerr << '[' << __FUNCTION__ << ']' << " cannot open file: " << p.path().string() << '\n';
                    throw exception();
                }

                files.push_back(p.path().string());

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
            cout << "void " << cs->name << "::Serialize(json &js, const Engine::Entity *entity) {\n";
            
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
            cout << "void " << cs->name << "::Deserialize(json &js, Engine::Entity *entity) {\n";

            cout << "  " << cs->name << " *e = (" << cs->name << " *)entity;\n";
            for (auto it = cs->properties.rbegin(); it != cs->properties.rend(); it++) {
                Property *p = *it;
                cout << "  e->" << p->name << " = js.back().get<" << p->type << ">(); js.erase(--js.end());\n";
            }

            if (!cs->base.empty()) {
                cout << "  " << cs->base << "::StaticType()->Deserialize(js, entity);\n";
            }

            cout << "}\n";
        }
    };
    for (Namespace *ns : namespaces) {
        write(ns);
    }
}

void Generator::TypeInit() {
    function<void(Namespace *ns)> write = [&write](Namespace *ns) {
        for (Namespace *n : ns->namespaces) {
            cout << "{\n  using namespace " << n->name << ";\n";
            write(n);
            cout << "}\n";
        }
        for (Class *cs : ns->classes) {
            cout << "  " << cs->name << "::StaticType(new Engine::Type(\""
                << cs->name << "\", "
                << "Engine::instantiate<" << cs->name << ", true>, "
                << cs->name << "::Serialize, "
                << cs->name << "::Deserialize));\n";
        }
    };

    cout << "void type_init() {\n";
    for (Namespace *ns : namespaces) {
        write(ns);
    }
    cout << "}\n";
}

void Generator::TypeClear() {
    function<void(Namespace *ns)> write = [&write](Namespace *ns) {
        for (Namespace *n : ns->namespaces) {
            cout << "{\n  using namespace " << n->name << ";\n";
            write(n);
            cout << "}\n";
        }
        for (Class *cs : ns->classes) {
            cout << "  delete " << cs->name << "::StaticType();\n";
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
    
    std::cout << "#include <nlohmann/json.hpp>\n";
    std::cout << "using json = nlohmann::json;\n";

    Serialize();
    Deserialize();
    TypeInit();
    TypeClear();
}