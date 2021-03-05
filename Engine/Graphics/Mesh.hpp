#pragma once

#include <GL/glew.h>

#include <EngineExport.hpp>
#include <Resource.hpp>

namespace Engine {
    
    class Model;

    /*
    Represents a wireframe that exists in the object space. 
    */
    class ENGINE_EXPORT Mesh : public Resource {
    private:
        Model *model;
        int index;

		unsigned vcnt;
		unsigned icnt;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;

    public:
        virtual ~Mesh() override;

        virtual void Init() override;

        friend class Camera;
    };
}

/*
#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include <Common/Resource.hpp>

#include <engine_global.hpp>

namespace Engine {

    class Model;

    SER_DECL(ENGINE_EXPORT, Mesh)

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
*/