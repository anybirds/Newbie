#include <iostream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_WINDOWS_UTF8
#include <stb_image.h>

#include <Graphics/Texture.hpp>
#include <Project.hpp>

using namespace std;

ATexture::ATexture() : 
    width(1024), height(1024), 
    internalFormat(TextureFormat::RGBA), format(TextureFormat::RGBA), dataType(TextureDataType::UNSIGNED_BYTE),
    horizontalWrap(TextureWrapMode::REPEAT), verticalWrap(TextureWrapMode::REPEAT),
    minFilter(TextureFilterMode::NEAREST), magFilter(TextureFilterMode::LINEAR) {}

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

    ATexture *atexture = (ATexture *)asset;
    path = atexture->GetPath();
    width = atexture->GetWidth();
    height = atexture->GetHeight();
    internalFormat = atexture->GetInternalFormat();
    format = atexture->GetFormat();
    dataType = atexture->GetDataType();
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
            backup.id = 0;
            throw exception();
        }

        TextureFormat format;
        switch (channel) {
        case 3:
            format = TextureFormat::RGB;
            break;
        case 4:
            format = TextureFormat::RGBA;
            break;
        default:
            format = TextureFormat::RGB;
            break;
        }

        this->width = width;
        this->height = height;
        this->internalFormat = format;
        this->format = format;
        this->dataType = TextureDataType::UNSIGNED_BYTE;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)GetHorizontalWrap());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)GetVerticalWrap());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)GetMinFilter());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)GetMagFilter());

    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)GetInternalFormat(), GetWidth(), GetHeight(), 0, (GLenum)GetFormat(), (GLenum)GetDataType(), image);

    if (image) {
        stbi_image_free(image);
    }

    if (glGetError() != GL_NO_ERROR) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot create Texture: " << GetName() << '\n';
        glDeleteTextures(1, &id);
        *this = backup;
        backup.id = 0;
        throw exception();
    }

    cerr << '[' << __FUNCTION__ << ']' << " created Texture: " << GetName() << '\n';
}