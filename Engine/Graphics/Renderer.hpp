#pragma once

#include <memory>

#include <Component.hpp>
#include <Type.hpp>
#include <IDraw.hpp>

namespace Engine {

	class Material;
	class Mesh;

	/*
	Responsible for rendering objects that have Mesh and Material.
	*/
	class ENGINE_EXPORT Renderer final : public Component, public IDraw {
		TYPE(Renderer)

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