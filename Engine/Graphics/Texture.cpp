#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Graphics/Texture.hpp>
#include <Project.hpp>

using namespace std;
using namespace Engine;

shared_ptr<Resource> ATexture::GetResource() {
    shared_ptr<Resource> sp;
    if (!(sp = resource.lock())) {
        try {
            sp = make_shared<Texture>(this);
        } catch(...) { }
        resource = sp;
    }
    return sp;
}

Texture::Texture(ATexture *atexture) : Resource(atexture) {
    Apply();
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::Apply() {
    glDeleteTextures(1, &id);

    ATexture *atexture = (ATexture *)asset;
    unsigned char *image;
    if (!GetPath().empty()) {
        stbi_set_flip_vertically_on_load(true);

        int width, height, channel;
        image = stbi_load((Project::GetInstance().GetDirectoy() + "/" + GetPath()).c_str(), &width, &height, &channel, 0);
        if (!image) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot load image file: " << atexture->GetPath() << '\n';
            throw exception();
        }

        GLenum format;
        switch (channel) {
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_RGB;
            break;
        }

        atexture->SetWidth(width);
        atexture->SetHeight(height);
        atexture->SetFormat(format);
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetHorizontalWrap());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetVerticalWrap());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetMinFilter());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetMagFilter());

    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)GetFormat(), GetWidth(), GetHeight(), 0, GetFormat(), GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
}