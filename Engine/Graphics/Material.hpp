#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <Asset.hpp>
#include <Resource.hpp>

class AShader;
class ATexture;
class Shader;
class Texture;

CLASS_ATTR(AMaterial, Asset, ENGINE_EXPORT) {
    TYPE(AMaterial);

    PROPERTY(unsigned, order, Order);
    PROPERTY(AShader *, vertexShader, VertexShader);
    PROPERTY(AShader *, fragmentShader, FragmentShader);
    // todo: move texture to drawer
    PROPERTY(ATexture *, mainTexture, MainTexture);

public:
    AMaterial();
    virtual std::shared_ptr<Resource> GetResource() override;
};

class ENGINE_EXPORT Material : public Resource {
    PROPERTY_GET(unsigned, order, Order);

private:
    std::shared_ptr<Shader> vertexShader;
    std::shared_ptr<Shader> fragmentShader;
    std::shared_ptr<Texture> mainTexture;
    
    GLuint program;

public:
    Material(AMaterial *amaterial);
    virtual ~Material();
    
    virtual void Apply() override;
    
    unsigned GetProgram() const { return program; }

    int GetInteger(const char *name) const;
    std::vector<int> GetIntegerArray(const char *name) const;
    float GetFloat(const char *name) const;
    std::vector<float> GetFloatArray(const char *name) const;
    glm::vec4 GetVector(const char *name) const;
    std::vector<glm::vec4> GetVectorArray(const char *name) const;
    glm::mat4 GetMatrix(const char *name) const;
    std::vector<glm::mat4> GetMatrixArray(const char *name) const;

    void SetInteger(const char *name, int value);
    void SetIntegerArray(const char *name, const int *value, int length);
    void SetFloat(const char *name, float value);
    void SetFloatArray(const char *name, const float *value, int length);
    void SetVector(const char *name, const glm::vec4 &value);
    void SetVectorArray(const char *name, const glm::vec4 *value, int length);
    void SetMatrix(const char *name, const glm::mat4 &value);
    void SetMatrixArray(const char *name, const glm::mat4 *value, int length);

    void UseTextures();

    std::shared_ptr<Shader> GetVertexShader() const { return vertexShader; }
    std::shared_ptr<Shader> GetFragmentShader() const { return fragmentShader; }
    std::shared_ptr<Texture> GetMainTexture() const { return mainTexture; }
};