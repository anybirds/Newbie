#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Graphics/Texture.hpp>

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

    int width, height, channel;
    unsigned char *image = stbi_load(GetPath().c_str(), &width, &height, &channel, 0);
    if (!image) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot load image file: " << GetPath() << '\n';
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

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
}