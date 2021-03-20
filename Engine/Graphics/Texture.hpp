#pragma once

#include <string>
#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>
#include <Type.hpp>

namespace Engine {
    
    class ENGINE_EXPORT [[Serialize]] ATexture : public Asset {
        TYPE(ATexture)
    
    private:
        std::string path;
        
    public:
        virtual std::shared_ptr<Resource> GetResource() override;
        const std::string &GetPath() const { return path; }
        
        void SetPath(const std::string &path) { this->path = path; }
    };

    /*
	Represents a texture that has been created by loading images.
	*/
    class ENGINE_EXPORT Texture : public Resource {
    private:
        GLuint id;
        
    public:
        Texture(ATexture *atexture);
        virtual ~Texture();
        
        virtual void Apply() override;

        const std::string &GetPath() const { ATexture *atexture = (ATexture *)asset; return atexture->GetPath(); }

        friend class Material;
    };
}