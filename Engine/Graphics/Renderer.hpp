#pragma once

#include <EngineExport.hpp>
#include <Entity.hpp>

namespace Engine {

	class Material;
	class Mesh;

	/*
	Responsible for rendering objects that have Mesh and Material.
	*/
	class ENGINE_EXPORT Renderer final : public Entity {
	private:
		Mesh *mesh;
		Material *material;

	public:
		Mesh *const GetMesh() const { return mesh; }
		Material *const GetMaterial() const { return material; }

		void SetMesh(Mesh *mesh) { this->mesh = mesh; }
		void SetMaterial(Material *material) { this->material = material; }
	};
}

/*
#ifndef RENDERER_H
#define RENDERER_H

#include <Common/Component.hpp>

#include <engine_global.hpp>

namespace Engine {

	class Material;
	class Mesh;

    SER_DECL(ENGINE_EXPORT, Renderer)
    class ENGINE_EXPORT Renderer final : public Component {
        TYPE_DECL(Renderer)

        PROPERTY(Mesh *, Mesh, mesh)
        PROPERTY(Material *, Material, material)

	public:
        Renderer(const std::string &name, Type *type = Renderer::type);
        virtual ~Renderer() override;
	};
}

typedef typename concat<TYPE_LIST, Engine::Renderer>::type TypeListRenderer;
#undef TYPE_LIST
#define TYPE_LIST TypeListRenderer

#endif
*/