#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include <Common/Resource.hpp>

#include <engine_global.hpp>

namespace Engine {

    class Model;

    SER_DECL(ENGINE_EXPORT, Mesh)

    /*
	Represents a wireframe that exists in the object space. 
	Users must define a MeshDetail object and pass it as an argument in order to create a Mesh.
    Mesh object can consist points in 2D and 3D space,
	can have multiple attributes for each vertex and can represent indexed wireframe. 
	*/
    class ENGINE_EXPORT Mesh final : public Resource {
        TYPE_DECL(Mesh)

	private:
        Model *model;
        int index;

		unsigned vcnt;
		unsigned icnt;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		
    public:
        Mesh(const std::string &name, Type *type = Mesh::type);

        virtual void OnInit() override;
        virtual void OnDestroy() override;

        friend class Camera;
	};
}

typedef typename concat<TYPE_LIST, Engine::Mesh>::type TypeListMesh;
#undef TYPE_LIST
#define TYPE_LIST TypeListMesh

#endif
