#include <fstream>
#include <iostream>
#include <sstream>

#include <Graphics/Shader.hpp>

using namespace std;
using namespace Engine;

shared_ptr<Resource> AShader::GetResource() {
    shared_ptr<Resource> sp;
    if (!(sp = resource.lock())) {
        try {
            sp = make_shared<Shader>(this);
        } catch(...) { }
        resource = sp;
    }
    return sp;
}

Shader::Shader(AShader *ashader) : Resource(ashader) {
    Apply();
}

Shader::~Shader() {
    glDeleteShader(id);
}

void Shader::Apply() {
    glDeleteShader(id);

    id = glCreateShader(GetShaderType());

    if (!id) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to create a new shader" << endl;
        throw exception();
    }

    ifstream file(GetPath());
    if (!file.is_open()) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to find a shader: " << GetPath() << endl;
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
        cerr << '[' << __FUNCTION__ << ']' << " failed to Compile a shader: " << GetPath() << endl;
        glDeleteShader(id);
        throw exception();
    }
}