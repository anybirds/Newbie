#pragma once

#include <memory>

#include <Component.hpp>

NAMESPACE(Engine) {

	class Material;
	class Mesh;
	class Renderer;

	CLASS_ATTR(Drawer, Component, ENGINE_EXPORT) {
		TYPE(Drawer);

		PROPERTY_GET(std::shared_ptr<Mesh>, mesh, Mesh);
		PROPERTY_GET(std::shared_ptr<Material>, material, Material);

	public:
		virtual void Draw(Renderer *renderer);

		virtual void OnTrack() override;
		virtual void OnUntrack() override;

		void SetMesh(const std::shared_ptr<Mesh> &mesh);
		void SetMaterial(const std::shared_ptr<Material> &material);
	};
}