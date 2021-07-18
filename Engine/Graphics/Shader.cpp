#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

#include <Graphics/Shader.hpp>
#include <Project.hpp>

using namespace std;

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
    Shader backup(*this);

    Resource::Apply();
    AShader *ashader = (AShader *)asset;
    path = ashader->GetPath();
    shaderType = ashader->GetShaderType();

    id = glCreateShader(GetShaderType());
    if (!id) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to create new Shader" << '\n';
        *this = backup;
        throw exception();
    }

    ifstream file;
    string absolute;
    Project &project = Project::GetInstance();
    if (project.IsLoaded()) {
        absolute = Project::GetInstance().GetDirectoy() + "/" + GetPath();
    } else {
        absolute = GetPath();
    }
    file.open(filesystem::u8path(absolute));
    if (!file.is_open()) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to find Shader: " << GetPath() << '\n';
        glDeleteShader(id);
        *this = backup;
        throw exception();
    }

    stringstream buf;
    buf << file.rdbuf();
    string source = buf.str();
    const char *source_char = source.c_str();
    glShaderSource(id, 1, &source_char, NULL);

    glCompileShader(id);
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (glGetError() != GL_NO_ERROR) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to create Shader: " << GetName() << '\n';
        glDeleteShader(id);
        *this = backup;
        throw exception();
    }
    if (status == GL_FALSE) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to compile Shader: " << GetPath() << '\n';
        string msg;
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        msg.resize(len);
        glGetShaderInfoLog(id, len, NULL, msg.data());
        cerr << msg << '\n';
        glDeleteShader(id);
        *this = backup;
        throw exception();
    }
    glDeleteShader(backup.id);
    cerr << '[' << __FUNCTION__ << ']' << " created Shader: " << GetName() << '\n';
}