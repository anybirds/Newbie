#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include <Scene.hpp>
#include <Graphics/Batch.hpp>
#include <Graphics/Material.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Texture.hpp>

using namespace std;
using namespace glm;

AMaterial::AMaterial() : vertexShader(nullptr), fragmentShader(nullptr) {}

shared_ptr<Resource> AMaterial::GetResource() {
    shared_ptr<Resource> sp;
    if (!(sp = resource.lock())) {
        try {
            sp = make_shared<Material>(this);
        } catch(...) { }
        resource = sp;
    }
    return sp;
}

Material::Material(AMaterial *amaterial) : Resource(amaterial), order(0), program(0) {
    Apply();
}

Material::~Material() {
    glDeleteProgram(program);
}

void Material::Apply() {
    Material backup(*this);

    AMaterial *amaterial = (AMaterial *)asset;
    vertexShader.reset();
    fragmentShader.reset();
    if (amaterial->GetVertexShader()) {
        vertexShader = dynamic_pointer_cast<Shader>(amaterial->GetVertexShader()->GetResource());
    }
    if (amaterial->GetFragmentShader()) {
        fragmentShader = dynamic_pointer_cast<Shader>(amaterial->GetFragmentShader()->GetResource());
    }
    if (!vertexShader) {
        cerr << '[' << __FUNCTION__ << ']' << " missing vertex shader in Material: " << GetName() << '\n';
        *this = backup;
        backup.program = 0;
        throw exception();
    }
    if (!fragmentShader) {
        cerr << '[' << __FUNCTION__ << ']' << " missing fragment shader in Material: " << GetName() << '\n';
        *this = backup;
        backup.program = 0;
        throw exception();
    }
    if (vertexShader->GetShaderType() != GL_VERTEX_SHADER ||
        fragmentShader->GetShaderType() != GL_FRAGMENT_SHADER) {
        cerr << '[' << __FUNCTION__ << ']' << " shader type mismatch in Material: " << GetName() << '\n';
        *this = backup;
        backup.program = 0;
        throw exception();
    }

    if (order != amaterial->GetOrder()) {
        SetOrder(amaterial->GetOrder());
    } 

    // attach shaders and link
    program = glCreateProgram();
    glAttachShader(program, vertexShader->GetId());
    glAttachShader(program, fragmentShader->GetId());
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (glGetError() != GL_NO_ERROR) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot create Material: " << GetName() << '\n';
        glDeleteProgram(program);
        *this = backup;
        backup.program = 0;
        throw exception();
    }
    if (status == GL_FALSE) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to link program Material: " << GetName() << '\n';
        string msg;
        int len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        msg.resize(len);
        glGetProgramInfoLog(program, len, NULL, msg.data());
        cerr << msg << '\n';
        glDeleteProgram(program);
        *this = backup;
        backup.program = 0;
        throw exception();
    }

    intMap = amaterial->GetIntMap();
    intArrayMap = amaterial->GetIntArrayMap();
    floatMap = amaterial->GetFloatMap();
    floatArrayMap = amaterial->GetFloatArrayMap();
    vec2Map = amaterial->GetVec2Map();
    vec2ArrayMap = amaterial->GetVec2ArrayMap();
    vec3Map = amaterial->GetVec3Map();
    vec3ArrayMap = amaterial->GetVec3ArrayMap();
    vec4Map = amaterial->GetVec4Map();
    vec4ArrayMap = amaterial->GetVec4ArrayMap();
    mat2Map = amaterial->GetMat2Map();
    mat2ArrayMap = amaterial->GetMat2ArrayMap();
    mat3Map = amaterial->GetMat3Map();
    mat3ArrayMap = amaterial->GetMat3ArrayMap();
    mat4Map = amaterial->GetMat4Map();
    mat4ArrayMap = amaterial->GetMat4ArrayMap();
    samplerMap.clear();
    for (auto &p : amaterial->GetSamplerMap()) {
        if (p.second) {
            samplerMap.insert({p.first, dynamic_pointer_cast<Texture>(p.second->GetResource())});
        }
    }
    ApplyUniforms();

    cerr << '[' << __FUNCTION__ << ']' << " created Material: " << GetName() << '\n';
}

void Material::SetOrder(unsigned order) {
    // batch reordering
    Scene &scene = Scene::GetInstance();
    auto it = scene.batches.find(this->order);
    if (it != scene.batches.end()) {
        vector<Batch *> batches;
        for (auto &p : it->second) {
            if (this == p.first.second) {
                batches.push_back(p.second);
            }
        }
        for (Batch *batch : batches) {
            it->second.erase(make_pair(batch->GetMesh(), batch->GetMaterial()));
        }
        if (it->second.empty()) {
            scene.batches.erase(it);
        }
        for (Batch *batch : batches) {
            scene.batches[order].insert(make_pair(make_pair(batch->GetMesh(), batch->GetMaterial()), batch));
        }
    }
    this->order = order;
} 

void Material::ApplyUniforms() {
    glUseProgram(program);
    GLint location;
    for (auto &p : intMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform1i(location, p.second);
    }
    for (auto &p : intArrayMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform1iv(location, (GLsizei)p.second.size(), p.second.data());
    }
    for (auto &p : floatMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform1f(location, p.second);
    }
    for (auto &p : floatArrayMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform1fv(location, (GLsizei)p.second.size(), p.second.data());
    }
    for (auto &p : vec2Map) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform2fv(location, 1, value_ptr(p.second));
    }
    for (auto &p : vec2ArrayMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform2fv(location, (GLsizei)p.second.size(), value_ptr(*p.second.data()));
    }
    for (auto &p : vec3Map) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform3fv(location, 1, value_ptr(p.second));
    }
    for (auto &p : vec3ArrayMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform3fv(location, (GLsizei)p.second.size(), value_ptr(*p.second.data()));
    }
    for (auto &p : vec4Map) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform4fv(location, 1, value_ptr(p.second));
    }
    for (auto &p : vec4ArrayMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform4fv(location, (GLsizei)p.second.size(), value_ptr(*p.second.data()));
    }
    for (auto &p : mat2Map) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix2fv(location, 1, GL_FALSE, value_ptr(p.second));
    }
    for (auto &p : mat2ArrayMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix2fv(location, (GLsizei)p.second.size(), GL_FALSE, value_ptr(*p.second.data()));
    }
    for (auto &p : mat3Map) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(p.second));
    }
    for (auto &p : mat3ArrayMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix3fv(location, (GLsizei)p.second.size(), GL_FALSE, value_ptr(*p.second.data()));
    }
    for (auto &p : mat4Map) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(p.second));
    }
    for (auto &p : mat4ArrayMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix4fv(location, (GLsizei)p.second.size(), GL_FALSE, value_ptr(*p.second.data()));
    }
    GLint i = 0;
    for (auto &p : samplerMap) {
        if (p.second) {
            location = glGetUniformLocation(program, p.first.c_str());
            glUniform1i(location, i);
            glActiveTexture(GL_TEXTURE0 + i++);
            glBindTexture(GL_TEXTURE_2D, p.second->GetId());
        }
    }

    glUseProgram(0);
}

void Material::UseProgramAndTextures() {
    glUseProgram(program);
    GLint i = 0;
    for (auto &p : samplerMap) {
        if (p.second) {
            GLuint location = glGetUniformLocation(program, p.first.c_str());
            glUniform1i(location, i);
            glActiveTexture(GL_TEXTURE0 + i++);
            glBindTexture(GL_TEXTURE_2D, p.second->GetId());
        }
    }
}

void Material::SetIntMap(const IntMap &intMap) {
    glUseProgram(program);
    for (auto &p : intMap) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniform1i(location, p.second);
    }
    this->intMap = intMap;
    glUseProgram(0);
}

void Material::SetIntArrayMap(const IntArrayMap &intArrayMap) {
    glUseProgram(program);
    for (auto &p : intArrayMap) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniform1iv(location, (GLsizei)p.second.size(), p.second.data());
    }
    this->intArrayMap = intArrayMap;
    glUseProgram(0);
}

void Material::SetFloatMap(const FloatMap &floatMap) {
    glUseProgram(program);
    for (auto &p : floatMap) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniform1f(location, p.second);
    }
    this->floatMap = floatMap;
    glUseProgram(0);
}

void Material::SetFloatArrayMap(const FloatArrayMap &floatArrayMap) {
    glUseProgram(program);
    for (auto &p : floatArrayMap) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniform1fv(location, (GLsizei)p.second.size(), p.second.data());
    }
    this->floatArrayMap = floatArrayMap;
    glUseProgram(0);
}

void Material::SetVec2Map(const Vec2Map &vec2Map) {
    glUseProgram(program);
    for (auto &p : vec2Map) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniform2fv(location, 1, value_ptr(p.second));
    }
    this->vec2Map = vec2Map;
    glUseProgram(0);
}

void Material::SetVec2ArrayMap(const Vec2ArrayMap &vec2ArrayMap) {
    glUseProgram(program);
    for (auto &p : vec2ArrayMap) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniform2fv(location, (GLsizei)p.second.size(), value_ptr(*p.second.data()));
    }
    this->vec2ArrayMap = vec2ArrayMap;
    glUseProgram(0);
}

void Material::SetVec3Map(const Vec3Map &vec3Map) {
    glUseProgram(program);
    for (auto &p : vec3Map) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniform3fv(location, 1, value_ptr(p.second));
    }
    this->vec3Map = vec3Map;
    glUseProgram(0);
}

void Material::SetVec3ArrayMap(const Vec3ArrayMap &vec3ArrayMap) {
    glUseProgram(program);
    for (auto &p : vec3ArrayMap) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniform3fv(location, (GLsizei)p.second.size(), value_ptr(*p.second.data()));
    }
    this->vec3ArrayMap = vec3ArrayMap;
    glUseProgram(0);
}

void Material::SetVec4Map(const Vec4Map &vec4Map) {
    glUseProgram(program);
    for (auto &p : vec4Map) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniform4fv(location, 1, value_ptr(p.second));
    }
    this->vec4Map = vec4Map;
    glUseProgram(0);
}

void Material::SetVec4ArrayMap(const Vec4ArrayMap &vec4ArrayMap) {
    glUseProgram(program);
    for (auto &p : vec4ArrayMap) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniform4fv(location, (GLsizei)p.second.size(), value_ptr(*p.second.data()));
    }
    this->vec4ArrayMap = vec4ArrayMap;
    glUseProgram(0);
}

void Material::SetMat2Map(const Mat2Map &mat2Map) {
    glUseProgram(program);
    for (auto &p : mat2Map) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix2fv(location, 1, GL_FALSE, value_ptr(p.second));
    }
    this->mat2Map = mat2Map;
    glUseProgram(0);
}

void Material::SetMat2ArrayMap(const Mat2ArrayMap &mat2ArrayMap) {
    glUseProgram(program);
    for (auto &p : mat2ArrayMap) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix2fv(location, (GLsizei)p.second.size(), GL_FALSE, value_ptr(*p.second.data()));
    }
    this->mat2ArrayMap = mat2ArrayMap;
    glUseProgram(0);
}

void Material::SetMat3Map(const Mat3Map &mat3Map) {
    glUseProgram(program);
    for (auto &p : mat3Map) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(p.second));
    }
    this->mat3Map = mat3Map;
    glUseProgram(0);
}

void Material::SetMat3ArrayMap(const Mat3ArrayMap &mat3ArrayMap) {
    glUseProgram(program);
    for (auto &p : mat3ArrayMap) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix3fv(location, (GLsizei)p.second.size(), GL_FALSE, value_ptr(*p.second.data()));
    }
    this->mat3ArrayMap = mat3ArrayMap;
    glUseProgram(0);
}

void Material::SetMat4Map(const Mat4Map &mat4Map) {
    glUseProgram(program);
    for (auto &p : mat4Map) {
        GLint location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(p.second));
    }
    this->mat4Map = mat4Map;
    glUseProgram(0);
}

void Material::SetMat4ArrayMap(const Mat4ArrayMap &mat4ArrayMap) {
    glUseProgram(program);
    for (auto &p : mat4ArrayMap) {
        GLint  location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix4fv(location, (GLsizei)p.second.size(), GL_FALSE, value_ptr(*p.second.data()));
    }
    this->mat4ArrayMap = mat4ArrayMap;
    glUseProgram(0);
}

int Material::GetInt(const string &name) const {
    auto it = intMap.find(name);
    return it == intMap.end() ? 0 : it->second;
}

vector<int> Material::GetIntArray(const string &name) const {
    auto it = intArrayMap.find(name);
    return it == intArrayMap.end() ? vector<int>{} : it->second;
}

float Material::GetFloat(const string &name) const {
    auto it = floatMap.find(name);
    return it == floatMap.end() ? 0.0f : it->second;
}

vector<float> Material::GetFloatArray(const string &name) const {
    auto it = floatArrayMap.find(name);
    return it == floatArrayMap.end() ? vector<float>{} : it->second;
}

vec2 Material::GetVec2(const string &name) const {
    auto it = vec2Map.find(name);
    return it == vec2Map.end() ? vec2{0.0f} : it->second;
}

vector<vec2> Material::GetVec2Array(const string &name) const {
    auto it = vec2ArrayMap.find(name);
    return it == vec2ArrayMap.end() ? vector<vec2>{} : it->second;
}

vec3 Material::GetVec3(const string &name) const {
    auto it = vec3Map.find(name);
    return it == vec3Map.end() ? vec3{0.0f} : it->second;
}

vector<vec3> Material::GetVec3Array(const string &name) const {
    auto it = vec3ArrayMap.find(name);
    return it == vec3ArrayMap.end() ? vector<vec3>{} : it->second;
}

vec4 Material::GetVec4(const string &name) const {
    auto it = vec4Map.find(name);
    return it == vec4Map.end() ? vec4{0.0f} : it->second;
}

vector<vec4> Material::GetVec4Array(const string &name) const {
    auto it = vec4ArrayMap.find(name);
    return it == vec4ArrayMap.end() ? vector<vec4>{} : it->second;
}

mat2 Material::GetMat2(const string &name) const {
    auto it = mat2Map.find(name);
    return it == mat2Map.end() ? mat2(1.0f) : it->second;
}

vector<mat2> Material::GetMat2Array(const string &name) const {
    auto it = mat2ArrayMap.find(name);
    return it == mat2ArrayMap.end() ? vector<mat2>{} : it->second;
}

mat3 Material::GetMat3(const string &name) const {
    auto it = mat3Map.find(name);
    return it == mat3Map.end() ? mat3(1.0f) : it->second;
}

vector<mat3> Material::GetMat3Array(const string &name) const {
    auto it = mat3ArrayMap.find(name);
    return it == mat3ArrayMap.end() ? vector<mat3>{} : it->second;
}

mat4 Material::GetMat4(const string &name) const {
    auto it = mat4Map.find(name);
    return it == mat4Map.end() ? mat4(1.0f) : it->second;
}

vector<mat4> Material::GetMat4Array(const string &name) const {
    auto it = mat4ArrayMap.find(name);
    return it == mat4ArrayMap.end() ? vector<mat4>{} : it->second;
}

shared_ptr<Texture> Material::GetSampler(const string &name) const {
    auto it = samplerMap.find(name);
    return it == samplerMap.end() ? nullptr : it->second;
}

void Material::SetInt(const string &name, int value) {
    intMap[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform1i(location, value);
    glUseProgram(0);
}

void Material::SetIntArray(const string &name, const vector<int> &value) {
    intArrayMap[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform1iv(location, (GLsizei)value.size(), value.data());
    glUseProgram(0);
}

void Material::SetFloat(const string &name, float value) {
    floatMap[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform1f(location, value);
    glUseProgram(0);
}

void Material::SetFloatArray(const string &name, const vector<float> &value) {
    floatArrayMap[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform1fv(location, (GLsizei)value.size(), value.data());
    glUseProgram(0);
}

void Material::SetVec2(const string &name, const vec2 &value) {
    vec2Map[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform2fv(location, 1, value_ptr(value));
    glUseProgram(0);
}

void Material::SetVec2Array(const string &name, const vector<vec2> &value) {
    vec2ArrayMap[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform2fv(location, (GLsizei)value.size(), value_ptr(*value.data()));
    glUseProgram(0);
}

void Material::SetVec3(const string &name, const vec3 &value) {
    vec3Map[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform3fv(location, 1, value_ptr(value));
    glUseProgram(0);
}

void Material::SetVec3Array(const string &name, const vector<vec3> &value) {
    vec3ArrayMap[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform3fv(location, (GLsizei)value.size(), value_ptr(*value.data()));
    glUseProgram(0);
}

void Material::SetVec4(const string &name, const vec4 &value) {
    vec4Map[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform4fv(location, 1, value_ptr(value));
    glUseProgram(0);
}

void Material::SetVec4Array(const string &name, const vector<vec4> &value) {
    vec4ArrayMap[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform4fv(location, (GLsizei)value.size(), value_ptr(*value.data()));
    glUseProgram(0);
}

void Material::SetMat2(const string &name, const mat2 &value) {
    mat2Map[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix2fv(location, 1, GL_FALSE, value_ptr(value));
    glUseProgram(0);
}

void Material::SetMat2Array(const string &name, const vector<mat2> &value) {
    mat2ArrayMap[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix2fv(location, (GLsizei)value.size(), GL_FALSE, value_ptr(*value.data()));
    glUseProgram(0);
}

void Material::SetMat3(const string &name, const mat3 &value) {
    mat3Map[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(value));
    glUseProgram(0);
}

void Material::SetMat3Array(const string &name, const vector<mat3> &value) {
    mat3ArrayMap[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix3fv(location, (GLsizei)value.size(), GL_FALSE, value_ptr(*value.data()));
    glUseProgram(0);
}

void Material::SetMat4(const string &name, const mat4 &value) {
    mat4Map[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
    glUseProgram(0);
}

void Material::SetMat4Array(const string &name, const vector<mat4> &value) {
    mat4ArrayMap[name] = value;
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(location, (GLsizei)value.size(), GL_FALSE, value_ptr(*value.data()));
    glUseProgram(0);
}

void Material::SetSampler(const string &name, const shared_ptr<Texture> &value) {
    samplerMap[name] = value;
}