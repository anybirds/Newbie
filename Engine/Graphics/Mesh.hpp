#pragma once

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

public:
    Mesh(AMesh *amesh);
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