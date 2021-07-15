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

Material::Material(AMaterial *amaterial) : Resource(amaterial), program(0) {
    Apply();
}

Material::~Material() {
    glDeleteProgram(program);
}

void Material::Apply() {
    Material backup(*this);

    Resource::Apply();
    AMaterial *amaterial = (AMaterial *)asset;
    if (amaterial->GetVertexShader()) {
        vertexShader = dynamic_pointer_cast<Shader>(amaterial->GetVertexShader()->GetResource());
    }
    if (amaterial->GetFragmentShader()) {
        fragmentShader = dynamic_pointer_cast<Shader>(amaterial->GetFragmentShader()->GetResource());
    }
    if (!vertexShader) {
        cerr << '[' << __FUNCTION__ << ']' << " missing vertex shader in Material: " << GetName() << '\n';
        *this = backup;
        throw exception();
    }
    if (!fragmentShader) {
        cerr << '[' << __FUNCTION__ << ']' << " missing fragment shader in Material: " << GetName() << '\n';
        *this = backup;
        throw exception();
    }
    if (vertexShader->GetShaderType() != GL_VERTEX_SHADER ||
        fragmentShader->GetShaderType() != GL_FRAGMENT_SHADER) {
        cerr << '[' << __FUNCTION__ << ']' << " shader type mismatch in Material: " << GetName() << '\n';
        *this = backup;
        throw exception();
    }

    if (order != amaterial->GetOrder()) {
        SetOrder(amaterial->GetOrder());
    } 

    intMap = amaterial->GetIntMap();
    intArrayMap = amaterial->GetIntArrayMap();
    floatMap = amaterial->GetFloatMap();
    floatArrayMap = amaterial->GetFloatArrayMap();
    vectorMap = amaterial->GetVectorMap();
    vectorArrayMap = amaterial->GetVectorArrayMap();
    matrixMap = amaterial->GetMatrixMap();
    matrixArrayMap = amaterial->GetMatrixArrayMap();
    for (auto &p : amaterial->GetSamplerMap()) {
        samplerMap.insert({p.first, dynamic_pointer_cast<Texture>(p.second->GetResource())});
    }

    // attach shaders and link
    program = glCreateProgram();
    glAttachShader(program, vertexShader->GetId());
    glAttachShader(program, fragmentShader->GetId());
    glLinkProgram(program);

    if (glGetError() != GL_NO_ERROR) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot create Material: " << GetName() << '\n';
        glDeleteProgram(program);
        *this = backup;
        throw exception();
    }

    glDeleteProgram(backup.program);
    cerr << '[' << __FUNCTION__ << ']' << " created Material: " << GetName() << '\n';
}

void Material::UpdateUniforms() {
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
    for (auto &p : vectorMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform4fv(location, 1, value_ptr(p.second));
    }
    for (auto &p : vectorArrayMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform4fv(location, (GLsizei)p.second.size(), value_ptr(*p.second.data()));
    }
    for (auto &p : matrixMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(p.second));
    }
    for (auto &p : matrixArrayMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniformMatrix4fv(location, (GLsizei)p.second.size(), GL_FALSE, value_ptr(*p.second.data()));
    }
    GLint i = 0;
    for (auto &p : samplerMap) {
        location = glGetUniformLocation(program, p.first.c_str());
        glUniform1i(location, i);
        glActiveTexture(GL_TEXTURE0 + i++);
        glBindTexture(GL_TEXTURE_2D, p.second->GetId());
    }
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

vec4 Material::GetVector(const string &name) const {
    auto it = vectorMap.find(name);
    return it == vectorMap.end() ? vec4{0.0f} : it->second;
}

vector<vec4> Material::GetVectorArray(const string &name) const {
    auto it = vectorArrayMap.find(name);
    return it == vectorArrayMap.end() ? vector<vec4>{} : it->second;
}
mat4 Material::GetMatrix(const string &name) const {
    auto it = matrixMap.find(name);
    return it == matrixMap.end() ? mat4(1.0f) : it->second;
}

vector<mat4> Material::GetMatrixArray(const string &name) const {
    auto it = matrixArrayMap.find(name);
    return it == matrixArrayMap.end() ? vector<mat4>{} : it->second;
}

shared_ptr<Texture> Material::GetSampler(const string &name) const {
    auto it = samplerMap.find(name);
    return it == samplerMap.end() ? nullptr : it->second;
}

void Material::SetInt(const string &name, int value) {
    intMap[name] = value;
}

void Material::SetIntArray(const string &name, const vector<int> &value) {
    intArrayMap[name] = value;
}

void Material::SetFloat(const string &name, float value) {
    floatMap[name] = value;
}

void Material::SetFloatArray(const string &name, const vector<float> &value) {
    floatArrayMap[name] = value;
}

void Material::SetVector(const string &name, const vec4 &value) {
    vectorMap[name] = value;
}

void Material::SetVectorArray(const string &name, const vector<vec4> &value) {
    vectorArrayMap[name] = value;
}

void Material::SetMatrix(const string &name, const mat4 &value) {
    matrixMap[name] = value;
}

void Material::SetMatrixArray(const string &name, const vector<mat4> &value) {
    matrixArrayMap[name] = value;
}

void Material::SetSampler(const string &name, const shared_ptr<Texture> &value) {
    samplerMap[name] = value;
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