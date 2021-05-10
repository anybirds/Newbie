#include <iostream>
#include <algorithm>

#include <Graphics/Texture.hpp>
#include <Graphics/Framebuffer.hpp>

using namespace std;

AFramebuffer::AFramebuffer() : colorTexture(nullptr), depthTexture(nullptr), useDepthRenderTexture(false) {}

shared_ptr<Resource> AFramebuffer::GetResource() {
    shared_ptr<Resource> sp;
    if (!(sp = resource.lock())) {
        try {
            sp = make_shared<Framebuffer>(this);
        } catch (...) {}
        resource = sp;
    }
    return sp;
}

Framebuffer::Framebuffer(AFramebuffer *aframebuffer) : Resource(aframebuffer), fbo(0U), rbo(0U) {
    Apply();
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(1, &rbo);
}

void Framebuffer::Apply() {
    Resource::Apply();
    AFramebuffer *aframebuffer = (AFramebuffer *)asset;
    useDepthRenderTexture = aframebuffer->GetUseDepthRenderTexture();
    if (aframebuffer->GetColorTexture()) {
        colorTexture = dynamic_pointer_cast<Texture>(aframebuffer->GetColorTexture()->GetResource());
    }
    if (aframebuffer->GetDepthTexture()) {
        depthTexture = dynamic_pointer_cast<Texture>(aframebuffer->GetDepthTexture()->GetResource());
    }

    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(1, &rbo);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    width = numeric_limits<int>::max(); height = width;
    if (colorTexture) {
        width = min(width, colorTexture->GetWidth()); height = min(height, colorTexture->GetHeight());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->GetId(), 0);
    } else {
        glDrawBuffer(GL_NONE);
    }

    if (colorTexture && GetUseDepthRenderTexture()) {
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    } else if (depthTexture) {
        width = min(width, depthTexture->GetWidth()); height = min(height, depthTexture->GetHeight());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->GetId(), 0);
    }
    maxWidth = width; maxHeight = height;

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cerr << '[' << __FUNCTION__ << ']' << " incomplete Framebuffer: " << GetName() << '\n';
        glDeleteFramebuffers(1, &fbo);
        throw exception();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (glGetError() != GL_NO_ERROR) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot create Framebuffer: " << GetName() << '\n';
        glDeleteFramebuffers(1, &fbo);
        throw exception();
    }
    cerr << '[' << __FUNCTION__ << ']' << " created Mesh: " << GetName() << '\n';
}

void Framebuffer::SetWidth(int width) {
    this->width = min(width, maxWidth);
}

void Framebuffer::SetHeight(int height) {
    this->height = min(height, maxHeight);
}

// todo: various format, type support
vector<uint8_t> Framebuffer::ReadPixels(int x, int y, int width, int height) {
    if (!colorTexture) {
        throw exception();
    }

    vector<uint8_t> ret;
    int channels;
    GLenum format;
    GLenum type = GL_UNSIGNED_BYTE;
    switch (colorTexture->GetFormat()) {
        case Texture::RGB:
            channels = 3;
            format = GL_RGB;
            break;
        case Texture::RGBA:
            channels = 4;
            format = GL_RGBA;
            break;
        default:
            throw exception();
            break;
    }
    ret.resize(width * height * channels);
    glReadPixels(x, y, width, height, format, type, &ret[0]);
    return ret;
}