#pragma once

#include <vector>
#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

class AModel;
class Model;

CLASS_ATTR(AMesh, Asset, ENGINE_EXPORT) {
    TYPE(AMesh);

    PROPERTY(AModel *, amodel, Model);
    PROPERTY(uint32_t, index, Index);

public:
    AMesh();
    virtual std::shared_ptr<Resource> GetResource() override;
};

CLASS_RESOURCE_ATTR(Mesh, Resource, ENGINE_EXPORT) {
    TYPE(Mesh);

    PROPERTY_GET(std::shared_ptr<Model>, model, Model);
    PROPERTY_GET(uint32_t, index, Index);
    PROPERTY_GET(unsigned, vcnt, VertexCount);
    PROPERTY_GET(unsigned, icnt, ElementCount);
    
private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    bool GenerateBuffers(const std::vector<float> &vert, const std::vector<unsigned> &attrib, const std::vector<unsigned> &idx);

public:
    enum {
        POSITION,
        NORMAL,
        UV,
        TANGENT
    };

    Mesh(AMesh *amesh);
    Mesh(const std::vector<float> &vert, const std::vector<unsigned> &attrib, const std::vector<unsigned> &idx);
    virtual ~Mesh();
    
    virtual void Apply() override;

    GLuint GetVertexArray() const { return vao; }
    GLuint GetVertexBuffer() const { return vbo; }
    GLuint GetElementBuffer() const { return ebo; }
    unsigned GetFaceCount() const { return icnt / 3; }
};