#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <Asset.hpp>
#include <Resource.hpp>

class AShader;
class ATexture;
class Shader;
class Texture;

using IntMap = std::unordered_map<std::string, int>;
using IntArrayMap = std::unordered_map<std::string, std::vector<int>>;
using FloatMap = std::unordered_map<std::string, float>;
using FloatArrayMap = std::unordered_map<std::string, std::vector<float>>;
using VectorMap = std::unordered_map<std::string, glm::vec4>;
using VectorArrayMap = std::unordered_map<std::string, std::vector<glm::vec4>>;
using MatrixMap = std::unordered_map<std::string, glm::mat4>;
using MatrixArrayMap = std::unordered_map<std::string, std::vector<glm::mat4>>;
using SamplerMap  = std::unordered_map<std::string, ATexture *>;

CLASS_ATTR(AMaterial, Asset, ENGINE_EXPORT) {
    TYPE(AMaterial);

    PROPERTY(unsigned, order, Order);
    PROPERTY(AShader *, vertexShader, VertexShader);
    PROPERTY(AShader *, fragmentShader, FragmentShader);

    PROPERTY(IntMap, intMap, IntMap);
    PROPERTY(IntArrayMap, intArrayMap, IntArrayMap);
    PROPERTY(FloatMap, floatMap, FloatMap);
    PROPERTY(FloatArrayMap, floatArrayMap, FloatArrayMap);
    PROPERTY(VectorMap, vectorMap, VectorMap);
    PROPERTY(VectorArrayMap, vectorArrayMap, VectorArrayMap);
    PROPERTY(MatrixMap, matrixMap, MatrixMap);
    PROPERTY(MatrixArrayMap, matrixArrayMap, MatrixArrayMap);
    PROPERTY(SamplerMap, samplerMap, SamplerMap);

public:
    AMaterial();
    virtual std::shared_ptr<Resource> GetResource() override;
};

class ENGINE_EXPORT Material : public Resource {
    PROPERTY_GET(unsigned, order, Order);

private:
    std::shared_ptr<Shader> vertexShader;
    std::shared_ptr<Shader> fragmentShader;
    
    IntMap intMap;
    IntArrayMap intArrayMap;
    FloatMap floatMap;
    FloatArrayMap floatArrayMap;
    VectorMap vectorMap;
    VectorArrayMap vectorArrayMap;
    MatrixMap matrixMap;
    MatrixArrayMap matrixArrayMap;
    std::unordered_map<std::string, std::shared_ptr<Texture>> samplerMap;
    
    GLuint program;

public:
    Material(AMaterial *amaterial);
    virtual ~Material();
    
    virtual void Apply() override;

    int GetInt(const std::string &name) const;
    std::vector<int> GetIntArray(const std::string &name) const;
    float GetFloat(const std::string &name) const;
    std::vector<float> GetFloatArray(const std::string &name) const;
    glm::vec4 GetVector(const std::string &name) const;
    std::vector<glm::vec4> GetVectorArray(const std::string &name) const;
    glm::mat4 GetMatrix(const std::string &name) const;
    std::vector<glm::mat4> GetMatrixArray(const std::string &name) const;
    std::shared_ptr<Texture> GetSampler(const std::string &name) const;

    void SetInt(const std::string &name, int value);
    void SetIntArray(const std::string &name, const std::vector<int> &value);
    void SetFloat(const std::string &name, float value);
    void SetFloatArray(const std::string &name, const std::vector<float> &value);
    void SetVector(const std::string &name, const glm::vec4 &value);
    void SetVectorArray(const std::string &name, const std::vector<glm::vec4> &value);
    void SetMatrix(const std::string &name, const glm::mat4 &value);
    void SetMatrixArray(const std::string &name, const std::vector<glm::mat4> &value);
    void SetSampler(const std::string &name, const std::shared_ptr<Texture> &value);

    void UpdateUniforms();

    std::shared_ptr<Shader> GetVertexShader() const { return vertexShader; }
    std::shared_ptr<Shader> GetFragmentShader() const { return fragmentShader; }
    GLuint GetProgram() const { return program; }
};