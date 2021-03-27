#pragma once

#include <memory>

#include <Graphics/Drawer.hpp>

namespace Engine {

	class Material;
	class Mesh;

	/*
	Responsible for rendering objects that have Mesh and Material.
	*/
	class ENGINE_EXPORT [[Serialize]] MeshDrawer final : public Drawer {
		TYPE(MeshDrawer)

	private:
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;

	public:
		virtual void Draw(Camera *camera) override;

		const std::shared_ptr<Mesh> &GetMesh() const { return mesh; }
		const std::shared_ptr<Material> &GetMaterial() const { return material; }

		void SetMesh(const std::shared_ptr<Mesh> &mesh) { this->mesh = mesh; }
		void SetMaterial(const std::shared_ptr<Material> &material) { this->material = material; }
	};
}