#pragma once

#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

namespace Engine {
    
    class AModel;
    class Model;

    class ENGINE_EXPORT [[Serialize]] AMesh : public Asset {
        TYPE(AMesh)

    private:
        AModel *amodel;
        uint32_t index;
    
    public:
        virtual std::shared_ptr<Resource> GetResource() override;

        AModel *GetModel() const { return amodel; }
        uint32_t GetIndex() const { return index; }

        void SetModel(AModel *amodel) { this->amodel = amodel; }
        void SetIndex(uint32_t index) { this->index = index; }
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