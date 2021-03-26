#include <iostream>
#include <algorithm>

#include <Graphics/Texture.hpp>
#include <Graphics/Framebuffer.hpp>

using namespace std;
using namespace Engine;

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

Framebuffer::Framebuffer(AFramebuffer *aframebuffer) : Resource(aframebuffer) {
    Apply();
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::Apply() {
    AFramebuffer *aframebuffer = (AFramebuffer *)asset;
    colorTexture = dynamic_pointer_cast<Texture>(aframebuffer->GetColorTexture()->GetResource());
    depthTexture = dynamic_pointer_cast<Texture>(aframebuffer->GetDepthTexture()->GetResource());

    glDeleteFramebuffers(1, &fbo);
    
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    width = numeric_limits<int>::max(); height = width;
    if (colorTexture) {
        width = min(width, colorTexture->GetWidth()); height = min(height, colorTexture->GetHeight());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->id, 0);
    } else {
        glDrawBuffer(GL_NONE);
    }
    if (depthTexture) {
        width = min(width, depthTexture->GetWidth()); height = min(height, depthTexture->GetHeight());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->id, 0);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glDeleteFramebuffers(1, &fbo);
        cerr << '[' << __FUNCTION__ << ']' << " incomplete Framebuffer: " << GetName() << '\n';
        throw exception();    
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}