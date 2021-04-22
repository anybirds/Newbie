#pragma once

#include <memory>

#include <Component.hpp>

NAMESPACE(Engine) {

	class Material;
	class Mesh;
	class Renderer;

	/*
	Responsible for rendering objects that have Mesh and Material.
	*/
	CLASS_ATTR(Drawer, Component, ENGINE_EXPORT) {
		TYPE(Drawer);

        // todo: shader properties
		
		PROPERTY(std::shared_ptr<Mesh>, mesh, Mesh);
		PROPERTY(std::shared_ptr<Material>, material, Material);

	public:
		virtual void Draw(Renderer *renderer);

		virtual void OnEnable() override;
		virtual void OnDisable() override;
	};
}