#include "utils.h"

namespace sunstorm
{
  namespace io 
  {
    std::string readFile(std::string filepath)
    {
      std::ifstream input(RES_DIR + filepath);

      if (input.fail() || !input.is_open()) {
        std::cerr << "[Error] Failed to open file: " << filepath << "!" << std::endl;
      }

      std::stringstream *buf = new std::stringstream();
      (*buf) << input.rdbuf();
      
      return buf->str();
    }
    
    gfx::Texture* readTextureFile(std::string filepath)
    {
      int w = 0;
      int h = 0;
      int comp;
      stbi_set_flip_vertically_on_load(true);
      unsigned char* image = stbi_load((RES_DIR + filepath).c_str(), &w, &h, &comp, STBI_rgb_alpha);

      if (image == nullptr) {
        std::cerr << "[Error] Failed to read image file: " << filepath << "! - " << stbi_failure_reason() << std::endl;
      }

      gfx::Texture* texture = new gfx::Texture(filepath, GL_TEXTURE_2D);
      texture->bind(0);
      texture->storeTexture2D(GL_RGBA, w, h, 0, image, GL_RGBA8);
      texture->genMipmaps();
      texture->unbind(0);
      stbi_image_free(image);
      return texture;
    }
  }
}