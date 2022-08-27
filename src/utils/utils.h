#pragma once

#include <unordered_map>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

#include "../graphics/graphics.h"

#define RES_DIR std::string("res/")

namespace sunstorm
{
  namespace io
  {
    /**
     * @brief Reads file into string buffer and returns content.
     * 
     * @param filepath 
     * @return std::string 
     */
    std::string readFile(std::string filepath);

    /**
     * @brief Reads image file into Texture object and stores image
     *    data in VRAM for sampling.
     * 
     * @param filepath 
     * @return gfx::Texture* 
     */
    gfx::Texture* readTextureFile(std::string filepath);

    /**
     * @brief Reads wavefront file and stores model information into OpenGL
     *    vertex array object to be rendered.
     * 
     * @param filepath 
     * @return gfx::Mesh*
     */
    gfx::Mesh* readOBJFile(std::string filepath);
  }

  namespace time
  {
    /**
     * @brief Get the current time since the UNIX epoch (Jan 1st 1970) in microseconds.
     * 
     * @return long long 
     */
    long long getTimeMicroseconds();
  }
}