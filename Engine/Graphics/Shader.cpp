#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include <Graphics/Shader.hpp>

using namespace std;
using namespace Engine;

namespace Engine {
    TYPE_DEF(Shader)
    SER_DEF(Shader, Resource,
    MEMBER_SER | MEMBER_SHOW, std::string, path,
    MEMBER_SER | MEMBER_SHOW, std::string, shaderType
    )
}

Shader::Shader(const string &name, Type *type) : Resource(name, type) {

}

void Shader::OnInit() {
    if (shaderType == "vertex") {
        id = glCreateShader(GL_VERTEX_SHADER);
    } else if (shaderType == "fragment") {
        id = glCreateShader(GL_FRAGMENT_SHADER);
    }

    if (!id) {
#ifdef DEBUG
        cout << '[' << __FUNCTION__ << ']' << " failed to create a new shader" << endl;
#endif
        throw exception();
    }

    ifstream file(path);
    if (!file.is_open()) {
#ifdef DEBUG
        cout << '[' << __FUNCTION__ << ']' << " failed to find a shader: " << path << endl;
#endif
        glDeleteShader(id);
        throw exception();
    }

    stringstream buf;
    buf << file.rdbuf();
    string source = buf.str();
    const char *source_char = source.c_str();
    glShaderSource(id, 1, &source_char, NULL);

    glCompileShader(id);
    GLint status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
#ifdef DEBUG
        cout << '[' << __FUNCTION__ << ']' << " failed to Compile a shader: " << path << endl;
#endif
        glDeleteShader(id);
        throw exception();
    }

    loaded = true;
    // shouldLoad = false;
}

void Shader::OnDestroy() {
    glDeleteShader(id);
    loaded = false;
}
