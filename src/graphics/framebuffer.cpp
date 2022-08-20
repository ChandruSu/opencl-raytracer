
#include "graphics.h"

namespace sunstorm
{
  namespace gfx
  {
    // ----- Frame Buffer ----- //

    Framebuffer::Framebuffer()
    {
      glGenFramebuffers(1, &framebufferId);
      SSRT_DBG_OUTPUT("Created Framebuffer");
    }
    
    Framebuffer::~Framebuffer()
    {
      SSRT_DBG_OUTPUT("Destroyed Framebuffer");
      glDeleteFramebuffers(1, &framebufferId);
    }

    void Framebuffer::bindFramebuffer() const
    {
      glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
    }

    void Framebuffer::unbindFramebuffer() const
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
  
    void Framebuffer::attachTexture(Texture* texture, GLenum attachment) const
    {
      glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture->getTextureId(), 0);
    }
  
    void Framebuffer::attachRenderbuffer(Renderbuffer* renderBuffer, GLenum target) const
    {
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, target, GL_RENDERBUFFER, renderBuffer->getRenderbufferId());
    }
  
    void Framebuffer::complete() const
    {
      bindFramebuffer();
      GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      unbindFramebuffer();

      if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "[Error] Invalid framebuffer! errCode=" << status << std::endl;
      }
    }

    // ---- Render Buffers ---- //

    Renderbuffer::Renderbuffer(int width, int height, GLenum internalFormat)
      : width(width), height(height), internalFormat(internalFormat)
    {
      glGenRenderbuffers(1, &renderbufferId);
      bindRenderbuffer();
      glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
      unbindRenderbuffer();
    }
    
    Renderbuffer::~Renderbuffer()
    {
      glDeleteRenderbuffers(1, &renderbufferId);
    }

    void Renderbuffer::bindRenderbuffer() const
    {
      glBindRenderbuffer(GL_RENDERBUFFER, renderbufferId);
    }

    void Renderbuffer::unbindRenderbuffer() const
    {
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
  }
}