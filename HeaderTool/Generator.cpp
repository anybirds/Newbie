#include <functional>
#include <filesystem>

#include <Generator.hpp>
#include <Namespace.hpp>
#include <Class.hpp>
#include <Enum.hpp>
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
            // don't serialize resource
            if (cs->resource) {
                continue;
            }

            cout << "void " << cs->name << "::Serialize(json &js, const Entity *entity) {\n";
            cout << "  js = \"[]\"_json;\n";
            
            if (!cs->base.empty()) {
                cout << "  " << cs->base << "::StaticType()->Serialize(js, entity);\n";
            }

            cout << "  const " << cs->name << " *e = (const " << cs->name << " *)entity;\n";
            for (Property *p : cs->properties) {
                cout << "  js.push_back(e->" << p->memberName << ");\n";
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
            // don't deserialize resource
            if (cs->resource) {
                continue;
            }

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
                cout << "  e->" << p->memberName << " = js[i++].get<" << p->type << ">();\n";
            }
            cout << "  return i;\n";

            cout << "}\n";
        }
    };
    for (Namespace *ns : namespaces) {
        write(ns);
    }
}

void Generator::Visualize() {
    function<void(Namespace *ns)> write = [&write](Namespace *ns) {
        for (Namespace *n : ns->namespaces) {
            cout << "namespace " << n->name << " {\n";
            write(n);
            cout << "}\n";
        }
        for (Class *cs : ns->classes) {
            cout << "void " << cs->name << "::Visualize(Entity *entity) {\n";

            if (!cs->base.empty()) {
                cout << "  " << cs->base << "::StaticType()->Visualize(entity);\n";
            }

            cout << "  " << cs->name << " *e = (" << cs->name << " *)entity;\n";
            for (size_t i = 0; i < cs->properties.size(); i++) {
                Property *p = cs->properties[i];
                switch (p->mod) {
                    case Property::DEFAULT:
                        cout << "  " << p->type << " " << p->memberName << " = e->" << "Get" << p->propertyName << "();\n";
                        cout << "  GetVisualizeProperty()(\"" << p->propertyName << "\", visualize<decltype(" << p->memberName << ")>, (void *)&" << p->memberName << ");\n";
                        cout << "  e->" << "Set" << p->propertyName << "(" << p->memberName << ");\n";
                        break;
                    case Property::BOOL:
                        cout << "  " << p->type << " " << p->memberName << " = e->" << "Is" << p->propertyName << "();\n";
                        cout << "  GetVisualizeProperty()(\"" << p->propertyName << "\", visualize<decltype(" << p->memberName << ")>, (void *)&" << p->memberName << ");\n";
                        cout << "  e->" << "Set" << p->propertyName << "(" << p->memberName << ");\n";
                        break;
                    case Property::GET:
                        cout << "  " << p->type << " " << p->memberName << " = e->" << "Get" << p->propertyName << "();\n";
                        cout << "  GetVisualizeProperty()(\"" << p->propertyName << "\", visualize<decltype(" << p->memberName << "), false>, (void *)&" << p->memberName << ");\n";
                        break;
                    case Property::IS:
                        cout << "  " << p->type << " " << p->memberName << " = e->" << "Is" << p->propertyName << "();\n";
                        cout << "  GetVisualizeProperty()(\"" << p->propertyName << "\", visualize<decltype(" << p->memberName << "), false>, (void *)&" << p->memberName << ");\n";
                        break;
                    default:
                        break;
                }
            }

            cout << "}\n";
        }
    };
    for (Namespace *ns : namespaces) {
        write(ns);
    }
}

void Generator::TypeInit() {
    vector<Namespace *> stack;
    function<void(Namespace *)> instantiate = [&instantiate, &stack](Namespace *ns) {
        for (Namespace *n : ns->namespaces) {
            stack.push_back(n);
            instantiate(n);
            stack.pop_back();
        }
        if (ns->classes.empty() && ns->enums.empty()) {
            return;
        }

        string prefix;
        for (Namespace *n : stack) {
            prefix += n->name;
            prefix += "::";
        }
        for (Class *cs : ns->classes) {
            string name = prefix + cs->name;
            cout << "  GetType<" << name << ">() = new Type(\"" << name << "\");\n";
        }
        for (Enum *e : ns->enums) {
            string name = prefix + e->name;
            cout << "  GetType<" << name << ">() = new Type(\"" << name << "\");\n";
        }
    };
    function<void(Namespace *)> write = [&write, &stack](Namespace *ns) {
        for (Namespace *n : ns->namespaces) {
            stack.push_back(n);
            write(n);
            stack.pop_back();
        }
        if (ns->classes.empty() && ns->enums.empty()) {
            return;
        }

        string prefix;
        for (Namespace *n : stack) {
            prefix += n->name;
            prefix += "::";
        }
        for (Class *cs : ns->classes) {
            string name = prefix + cs->name;
            string candidate; // candidate for base class namespace
            // in order to compile, only one should pass the if statement
            cout << "  if (Type *base = Type::GetType(\"" << cs->base << "\")) { " << name << "::StaticType()->SetBase(base); }\n";
            for (Namespace *n : stack) {
                candidate += n->name;
                candidate += "::";
                cout << "  if (Type *base = Type::GetType(\"" << candidate << cs->base << "\")) { " << name << "::StaticType()->SetBase(base); }\n";
            }
            if (!cs->resource) {
                cout << "  " << name << "::StaticType()->SetInstantiate(" << "instantiate<" << name << ", true>);\n";
                cout << "  " << name << "::StaticType()->SetSerialize(" << name << "::Serialize);\n";
                cout << "  " << name << "::StaticType()->SetDeserialize(" << name << "::Deserialize);\n";
            }
            cout << "  " << name << "::StaticType()->SetVisualize(" << name << "::Visualize);\n";
            cout << "  " << name << "::StaticType()->SetAbstract(std::is_abstract<" << name << ">::value);\n";
            
            // class blueprint
            cout << "  {\n";
            for (Property *p : cs->properties) {
                cout << "  " << p->type << ' ' << p->memberName << "{};\n"; 
            }
            cout << "  " << name << "::StaticType()->blueprint = {\n";
            for (Property *p : cs->properties) {
                cout << "  { \"" << p->type << "\", \"" << p->propertyName << "\", " << p->memberName << " },\n";
            }
            cout << "  };\n  }\n";
        }
        for (Enum *e : ns->enums) {
            string name = prefix + e->name;

            cout << "  GetType<" << name << ">()->SetEnum(true);\n";
            // enum blueprint
            cout << "  GetType<" << name << ">()->blueprint = {\n";
            string base = "0";
            uint64_t cnt = 0;
            for (auto &p : e->enums) {
                if (!p.second.empty()) {
                    base = p.second;
                    cnt = 0;
                }
                cout << "  { \"" << p.first << "\", static_cast<" << e->base << ">(" << base << " + " << cnt++ << ") },\n";
            }
            cout << "  };\n";
        }
    };

    cout << "void type_init() {\n";
    for (Namespace *ns : namespaces) {
        instantiate(ns);
    }
    for (Namespace *ns : namespaces) {
        write(ns);
    }
    cout << "}\n";
}

void Generator::TypeClear() {
    vector<Namespace *> stack;
    function<void(Namespace *)> write = [&write, &stack](Namespace *ns) {
        for (Namespace *n : ns->namespaces) {
            stack.push_back(n);
            write(n);
            stack.pop_back();
        }
        if (ns->classes.empty() && ns->enums.empty()) {
            return;
        }

        string prefix;
        for (Namespace *n : stack) {
            prefix += n->name;
            prefix += "::";
        }
        for (Class *cs : ns->classes) {
            string name = prefix + cs->name;
            cout << "  delete " << name << "::StaticType();\n";
        }
        for (Enum *e : ns->enums) {
            string name = prefix + e->name;
            cout << "  delete GetType<" << name << ">();\n";
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
    
    cout << "#include <Visualize.hpp>\n";
    cout << "#include <nlohmann/json.hpp>\n";
    cout << "using json = nlohmann::json;\n";

    Serialize();
    Deserialize();
    Visualize();
    TypeInit();
    TypeClear();
}