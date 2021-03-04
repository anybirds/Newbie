#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <GL/glew.h>

#include <Common/Resource.hpp>

#include <engine_global.hpp>

namespace Engine {

    SER_DECL(ENGINE_EXPORT, Texture)

    /*
	Represents a texture that has been created by loading images.
	*/
    class ENGINE_EXPORT Texture final : public Resource {
        TYPE_DECL(Texture)

        PROPERTY(std::string, Path, path)

	private:
		GLuint id;

    public:
        Texture(const std::string &name, Type *type = Texture::type);

        void OnInit() override;
        void OnDestroy() override;

        friend class Material;
	};
}

typedef typename concat<TYPE_LIST, Engine::Texture>::type TypeListTexture;
#undef TYPE_LIST
#define TYPE_LIST TypeListTexture

#endif
