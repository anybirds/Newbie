#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include <Common/Component.hpp>

#include <engine_global.hpp>

namespace Engine {

    class Renderer;

    SER_DECL(ENGINE_EXPORT, Camera)

    /*
    Featured by its normalization matrix.
    Specify camera properties that affect the normalization matrix.
    */
    class ENGINE_EXPORT Camera final : public Component {
        TYPE_DECL(Camera)

    private:
        static std::unordered_set<Renderer *> rendset;

    public:
        static Camera *GetMainCamera();
        static void SetMainCamera(Camera *camera);

    private:
        glm::mat4 normalization;

        bool orthographic;
        float fovy;
        float width;
        float height;
#if defined(near)
#undef near
#endif
#if defined(far)
#undef far
#endif
        float near;
        float far;
        float left;
        float right;
        float bottom;
        float top;

	public:
        Camera(const std::string &name, Type *type = Camera::type);
        virtual ~Camera();

        virtual void OnInit() override;

        void ComputeNormalization();

        bool IsOrthographic() { return orthographic; }
        bool IsPerspective() { return !orthographic; }
        void SetOrthographic() { orthographic = true; ComputeNormalization(); }
        void SetPerspective() { orthographic = false; ComputeNormalization(); }

        GET(float, Fovy, fovy)
        GET(float, Width, width)
        GET(float, Height, height)
        GET(float, Near, near)
        GET(float, Far, far)
        GET(float, Left, left)
        GET(float, Right, right)
        GET(float, Top, top)
        GET(float, Bottom, bottom)

        void SetFovy(float fovy);
        void SetWidth(float width);
        void SetHeight(float height);
        void SetNear(float near);
        void SetFar(float far);
        void SetLeft(float left);
        void SetRight(float right);
        void SetTop(float top);
        void SetBottom(float bottom);

        void Render();

        friend class Renderer;
	};
}

typedef typename concat<TYPE_LIST, Engine::Camera>::type TypeListCamera;
#undef TYPE_LIST
#define TYPE_LIST TypeListCamera

#endif
