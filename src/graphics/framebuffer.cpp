
#include "graphics.h"

namespace sunstorm
{
  namespace gfx
  {
    // ----- Frame Buffer ----- //

    Framebuffer::Framebuffer(int width, int height) : width(width), height(height)
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
  
    void Framebuffer::attachTexture(const Texture& texture, GLenum attachment) const
    {
      glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture.getTextureId(), 0);
    }
  
    void Framebuffer::attachRenderbuffer(const Renderbuffer& renderBuffer, GLenum attachment) const
    {
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer.getRenderbufferId());
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
    
    void Framebuffer::drawTo(const Framebuffer& target) const
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferId);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.getFramebufferId());
      glDrawBuffer(GL_BACK);
      glBlitFramebuffer(0, 0, width, height, 0, 0, target.width, target.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
      glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
    
    void Framebuffer::draw(int width, int height) const
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferId);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
      glDrawBuffer(GL_BACK);
      glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
      glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
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