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
using Vec2Map = std::unordered_map<std::string, glm::vec2>;
using Vec2ArrayMap = std::unordered_map<std::string, std::vector<glm::vec2>>;
using Vec3Map = std::unordered_map<std::string, glm::vec3>;
using Vec3ArrayMap = std::unordered_map<std::string, std::vector<glm::vec3>>;
using Vec4Map = std::unordered_map<std::string, glm::vec4>;
using Vec4ArrayMap = std::unordered_map<std::string, std::vector<glm::vec4>>;
using Mat2Map = std::unordered_map<std::string, glm::mat2>;
using Mat2ArrayMap = std::unordered_map<std::string, std::vector<glm::mat2>>;
using Mat3Map = std::unordered_map<std::string, glm::mat3>;
using Mat3ArrayMap = std::unordered_map<std::string, std::vector<glm::mat3>>;
using Mat4Map = std::unordered_map<std::string, glm::mat4>;
using Mat4ArrayMap = std::unordered_map<std::string, std::vector<glm::mat4>>;
using SamplerMap  = std::unordered_map<std::string, ATexture *>;
using SamplerResourceMap = std::unordered_map<std::string, std::shared_ptr<Texture>>;

CLASS_ATTR(AMaterial, Asset, ENGINE_EXPORT) {
    TYPE(AMaterial);

    PROPERTY(unsigned, order, Order);
    PROPERTY(AShader *, vertexShader, VertexShader);
    PROPERTY(AShader *, fragmentShader, FragmentShader);

    PROPERTY(IntMap, intMap, IntMap);
    PROPERTY(IntArrayMap, intArrayMap, IntArrayMap);
    PROPERTY(FloatMap, floatMap, FloatMap);
    PROPERTY(FloatArrayMap, floatArrayMap, FloatArrayMap);
    PROPERTY(Vec2Map, vec2Map, Vec2Map);
    PROPERTY(Vec2ArrayMap, vec2ArrayMap, Vec2ArrayMap);
    PROPERTY(Vec3Map, vec3Map, Vec3Map);
    PROPERTY(Vec3ArrayMap, vec3ArrayMap, Vec3ArrayMap);
    PROPERTY(Vec4Map, vec4Map, Vec4Map);
    PROPERTY(Vec4ArrayMap, vec4ArrayMap, Vec4ArrayMap);
    PROPERTY(Mat2Map, mat2Map, Mat2Map);
    PROPERTY(Mat2ArrayMap, mat2ArrayMap, Mat2ArrayMap);
    PROPERTY(Mat3Map, mat3Map, Mat3Map);
    PROPERTY(Mat3ArrayMap, mat3ArrayMap, Mat3ArrayMap);
    PROPERTY(Mat4Map, mat4Map, Mat4Map);
    PROPERTY(Mat4ArrayMap, mat4ArrayMap, Mat4ArrayMap);
    PROPERTY(SamplerMap, samplerMap, SamplerMap);

public:
    AMaterial();
    virtual std::shared_ptr<Resource> GetResource() override;
};

CLASS_RESOURCE_ATTR(Material, Resource, ENGINE_EXPORT) {
    TYPE(Material);

    PROPERTY(unsigned, order, Order);
    PROPERTY_GET(std::shared_ptr<Shader>, vertexShader, VertexShader);
    PROPERTY_GET(std::shared_ptr<Shader>, fragmentShader, FragmentShader);

    PROPERTY(IntMap, intMap, IntMap);
    PROPERTY(IntArrayMap, intArrayMap, IntArrayMap);
    PROPERTY(FloatMap, floatMap, FloatMap);
    PROPERTY(FloatArrayMap, floatArrayMap, FloatArrayMap);
    PROPERTY(Vec2Map, vec2Map, Vec2Map);
    PROPERTY(Vec2ArrayMap, vec2ArrayMap, Vec2ArrayMap);
    PROPERTY(Vec3Map, vec3Map, Vec3Map);
    PROPERTY(Vec3ArrayMap, vec3ArrayMap, Vec3ArrayMap);
    PROPERTY(Vec4Map, vec4Map, Vec4Map);
    PROPERTY(Vec4ArrayMap, vec4ArrayMap, Vec4ArrayMap);
    PROPERTY(Mat2Map, mat2Map, Mat2Map);
    PROPERTY(Mat2ArrayMap, mat2ArrayMap, Mat2ArrayMap);
    PROPERTY(Mat3Map, mat3Map, Mat3Map);
    PROPERTY(Mat3ArrayMap, mat3ArrayMap, Mat3ArrayMap);
    PROPERTY(Mat4Map, mat4Map, Mat4Map);
    PROPERTY(Mat4ArrayMap, mat4ArrayMap, Mat4ArrayMap);
    PROPERTY(SamplerResourceMap, samplerMap, SamplerMap);
    
    PROPERTY_GET(GLuint, program, Program);

public:
    using asset_type = AMaterial;
    
    Material(AMaterial *amaterial);
    virtual ~Material();
    
    virtual void Apply() override;

    void SetOrder(unsigned order);
    void ApplyUniforms();
    void UseProgramAndTextures();
    
    void SetIntMap(const IntMap &intMap);
    void SetIntArrayMap(const IntArrayMap &intArrayMap);
    void SetFloatMap(const FloatMap &floatMap);
    void SetFloatArrayMap(const FloatArrayMap &floatArrayMap);
    void SetVec2Map(const Vec2Map &vec2Map);
    void SetVec2ArrayMap(const Vec2ArrayMap &vec2ArrayMap);
    void SetVec3Map(const Vec3Map &vec3Map);
    void SetVec3ArrayMap(const Vec3ArrayMap &vec3ArrayMap);
    void SetVec4Map(const Vec4Map &vec4Map);
    void SetVec4ArrayMap(const Vec4ArrayMap &vec4ArrayMap);
    void SetMat2Map(const Mat2Map &mat2Map);
    void SetMat2ArrayMap(const Mat2ArrayMap &mat2ArrayMap);
    void SetMat3Map(const Mat3Map &mat3Map);
    void SetMat3ArrayMap(const Mat3ArrayMap &mat3ArrayMap);
    void SetMat4Map(const Mat4Map &mat4Map);
    void SetMat4ArrayMap(const Mat4ArrayMap &mat4ArrayMap);

    int GetInt(const std::string &name) const;
    std::vector<int> GetIntArray(const std::string &name) const;
    float GetFloat(const std::string &name) const;
    std::vector<float> GetFloatArray(const std::string &name) const;
    glm::vec2 GetVec2(const std::string &name) const;
    std::vector<glm::vec2> GetVec2Array(const std::string &name) const;
    glm::vec3 GetVec3(const std::string &name) const;
    std::vector<glm::vec3> GetVec3Array(const std::string &name) const;
    glm::vec4 GetVec4(const std::string &name) const;
    std::vector<glm::vec4> GetVec4Array(const std::string &name) const;
    glm::mat2 GetMat2(const std::string &name) const;
    std::vector<glm::mat2> GetMat2Array(const std::string &name) const;
    glm::mat3 GetMat3(const std::string &name) const;
    std::vector<glm::mat3> GetMat3Array(const std::string &name) const;
    glm::mat4 GetMat4(const std::string &name) const;
    std::vector<glm::mat4> GetMat4Array(const std::string &name) const;
    std::shared_ptr<Texture> GetSampler(const std::string &name) const;

    void SetInt(const std::string &name, int value);
    void SetIntArray(const std::string &name, const std::vector<int> &value);
    void SetFloat(const std::string &name, float value);
    void SetFloatArray(const std::string &name, const std::vector<float> &value);
    void SetVec2(const std::string &name, const glm::vec2 &value);
    void SetVec2Array(const std::string &name, const std::vector<glm::vec2> &value);
    void SetVec3(const std::string &name, const glm::vec3 &value);
    void SetVec3Array(const std::string &name, const std::vector<glm::vec3> &value);
    void SetVec4(const std::string &name, const glm::vec4 &value);
    void SetVec4Array(const std::string &name, const std::vector<glm::vec4> &value);
    void SetMat2(const std::string &name, const glm::mat2 &value);
    void SetMat2Array(const std::string &name, const std::vector<glm::mat2> &value);
    void SetMat3(const std::string &name, const glm::mat3 &value);
    void SetMat3Array(const std::string &name, const std::vector<glm::mat3> &value);
    void SetMat4(const std::string &name, const glm::mat4 &value);
    void SetMat4Array(const std::string &name, const std::vector<glm::mat4> &value);
    void SetSampler(const std::string &name, const std::shared_ptr<Texture> &value);
};