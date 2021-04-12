#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

#include <Graphics/Shader.hpp>
#include <Project.hpp>

using namespace std;
using namespace Engine;

AShader::AShader() : shaderType(Shader::VERTEX) {}

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

Shader::Shader(AShader *ashader) : Resource(ashader), id(0) {
    Apply();
}

Shader::~Shader() {
    glDeleteShader(id);
}

void Shader::Apply() {
    glDeleteShader(id);
    id = glCreateShader(GetShaderType());
    if (!id) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to create a new shader" << '\n';
        throw exception();
    }

    ifstream file(filesystem::u8path(Project::GetInstance().GetDirectoy() + "/" + GetPath()));
    if (!file.is_open()) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to find a shader: " << GetPath() << '\n';
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
        cerr << '[' << __FUNCTION__ << ']' << " failed to Compile a shader: " << GetPath() << '\n';
        glDeleteShader(id);
        throw exception();
    }

    if (glGetError() != GL_NO_ERROR) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot create Shader: " << GetName() << '\n';
        glDeleteShader(id);
        throw exception();
    }
    cerr << '[' << __FUNCTION__ << ']' << " created Shader: " << GetName() << '\n';
}