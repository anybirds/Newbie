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

class ENGINE_EXPORT Mesh : public Resource {
    PROPERTY_GET(uint32_t, index, Index);

private:
    std::shared_ptr<Model> model;
    
    unsigned vcnt;
    unsigned icnt;

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
    unsigned GetVertexCount() const { return vcnt; }
    unsigned GetElementCount() const { return icnt; }
    unsigned GetFaceCount() const { return icnt / 3; }
    std::shared_ptr<Model> GetModel() const { return model; }
};