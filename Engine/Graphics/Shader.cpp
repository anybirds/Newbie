#include <fstream>
#include <iostream>
#include <sstream>

#include <Graphics/Shader.hpp>

using namespace std;
using namespace Engine;

Shader::~Shader() {
    glDeleteShader(id);
    /*
    loaded = false;
    */
}

void Shader::Init() {
    if (shaderType == "vertex") {
        id = glCreateShader(GL_VERTEX_SHADER);
    } else if (shaderType == "fragment") {
        id = glCreateShader(GL_FRAGMENT_SHADER);
    }

    if (!id) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to create a new shader" << endl;
        throw exception();
    }

    ifstream file(path);
    if (!file.is_open()) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to find a shader: " << path << endl;
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
        cerr << '[' << __FUNCTION__ << ']' << " failed to Compile a shader: " << path << endl;
        glDeleteShader(id);
        throw exception();
    }

    /*
    loaded = true;
    // shouldLoad = false;
    */
}
