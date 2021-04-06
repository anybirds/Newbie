#pragma once

#include <memory>

#include <Graphics/Drawer.hpp>

NAMESPACE(Engine) {

	class Material;
	class Mesh;

	/*
	Responsible for rendering objects that have Mesh and Material.
	*/
	CLASS_FINAL_ATTR(MeshDrawer, Drawer, ENGINE_EXPORT) {
		TYPE(MeshDrawer);

		PROPERTY(std::shared_ptr<Mesh>, mesh, Mesh);
		PROPERTY(std::shared_ptr<Material>, material, Material);

	public:
		virtual void Draw(Camera *camera) override;
	};
}