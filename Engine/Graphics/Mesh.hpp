#pragma once

#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

NAMESPACE(Engine) {
    
    class AModel;
    class Model;

    CLASS_ATTR(AMesh, Asset, ENGINE_EXPORT) {
        TYPE(AMesh);

        PROPERTY(AModel *, amodel, Model);
        PROPERTY(uint32_t, index, Index);

    public:
        virtual std::shared_ptr<Resource> GetResource() override;
    };

    /*
    Represents a wireframe in the object space.
    */
    class ENGINE_EXPORT Mesh : public Resource {
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

        unsigned GetVertexCount() const { return vcnt; }
        unsigned GetFaceCount() const { return icnt / 3; }
        uint32_t GetIndex() const { AMesh *amesh = (AMesh *)asset; return amesh->GetIndex(); }
        std::shared_ptr<Model> GetModel() const { return model; }

        friend class MeshDrawer;
    };
}