#include <iostream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_WINDOWS_UTF8
#include <stb_image.h>

#include <Graphics/Texture.hpp>
#include <Project.hpp>

using namespace std;

ATexture::ATexture() : 
    width(1024), height(1024), format(Texture::RGBA), 
    horizontalWrap(Texture::REPEAT), verticalWrap(Texture::REPEAT),
    minFilter(Texture::NEAREST), magFilter(Texture::LINEAR) {}

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

Texture::Texture(ATexture *atexture) : Resource(atexture), id(0) {
    Apply();
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::Apply() {
    Texture backup(*this);

    Resource::Apply();
    ATexture *atexture = (ATexture *)asset;
    path = atexture->GetPath();
    width = atexture->GetWidth();
    height = atexture->GetHeight();
    format = atexture->GetFormat();
    horizontalWrap = atexture->GetHorizontalWrap();
    verticalWrap = atexture->GetVerticalWrap();
    minFilter = atexture->GetMinFilter();
    magFilter = atexture->GetMagFilter();

    unsigned char *image = nullptr;
    if (!GetPath().empty()) {
        stbi_set_flip_vertically_on_load(true);

        int width, height, channel;

        string absolute;
        Project &project = Project::GetInstance();
        if (project.IsLoaded()) {
            absolute = project.GetDirectoy() + "/" + GetPath();
        } else {
            absolute = GetPath();
        }
        image = stbi_load(absolute.c_str(), &width, &height, &channel, 0);
        if (!image) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot load image file: " << atexture->GetPath() << '\n';
            *this = backup;
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

    if (image) {
        stbi_image_free(image);
    }

    if (glGetError() != GL_NO_ERROR) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot create Texture: " << GetName() << '\n';
        glDeleteTextures(1, &id);
        *this = backup;
        throw exception();
    }

    glDeleteTextures(1, &backup.id);
    cerr << '[' << __FUNCTION__ << ']' << " created Texture: " << GetName() << '\n';
}