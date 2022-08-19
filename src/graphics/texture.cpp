
#include "graphics.h"


namespace sunstorm
{
  namespace gfx
  {
    Texture::Texture(std::string name, GLenum type) : name(name), target(type)
    {
      glGenTextures(1, &textureId);
      bind(0);
      setTexParamI(GL_TEXTURE_WRAP_S, GL_REPEAT);
      setTexParamI(GL_TEXTURE_WRAP_T, GL_REPEAT);
      setTexParamI(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
      setTexParamI(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      unbind(0);
      SSRT_DBG_OUTPUT("Created Texture: " + name);
    }
    
    Texture::~Texture()
    {
      glDeleteTextures(1, &textureId);
      SSRT_DBG_OUTPUT("Destroyed Texture: " + name);
    }  

    void Texture::bind(int i) const
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(target, textureId);
    }

    void Texture::unbind(int i) const
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(target, 0);
    }
    
    void Texture::setTexParamI(GLenum parameterName, GLint value) const
    {
      glTexParameteri(target, parameterName, value);
    }

    void Texture::genMipmaps() const
    {
      glTexParameterf(target, GL_TEXTURE_LOD_BIAS, -1.0f);
      glGenerateMipmap(target);
    }
    
    void Texture::storeTexture2D(GLint format, int w, int h, int level, unsigned char* image, GLint internalFormat) const
    {
      glTexImage2D(target, level, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, image);
    }
  }
}
