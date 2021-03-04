#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <Common/Resource.hpp>

#include <engine_global.hpp>

namespace Engine {

    class Shader;
    class Texture;

    SER_DECL(ENGINE_EXPORT, Material)

    class ENGINE_EXPORT Material final : public Resource {
        TYPE_DECL(Material)

        PROPERTY(Shader *, VertexShader, vertexShader)
        PROPERTY(Shader *, FragmentShader, fragmentShader)
        PROPERTY(Texture *, MainTexture, mainTexture)

    private:
        GLuint program;

    public:
        Material(const std::string &name, Type *type = Material::type);

        void OnInit() override;
        void OnDestroy() override;

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

        friend class Camera;
    };
}

typedef typename concat<TYPE_LIST, Engine::Material>::type TypeListMaterial;
#undef TYPE_LIST
#define TYPE_LIST TypeListMaterial

#endif
