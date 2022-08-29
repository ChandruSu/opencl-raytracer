#include "utils.h"

namespace sunstorm
{
  namespace io 
  {
    void splitString(std::string str, std::vector<std::string>& out, char delimeter)
    {
      out.clear();
      size_t i0 = 0;
      size_t i1 = i0;

      while (i1 <= str.length()) {
        if (str[i1] == delimeter || str[i1] == '\0') {
          out.push_back(str.substr(i0, i1 - i0));
          i0 = ++i1;
        }
        i1++;
      }
    }

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
      texture->storeTexture2D(w, h, 0, image);
      texture->genMipmaps();
      texture->unbind(0);
      stbi_image_free(image);
      return texture;
    }
    
    gfx::Mesh* readOBJFile(std::string filepath)
    {
      std::ifstream input(RES_DIR + filepath);

      if (input.fail() || !input.is_open()) {
        std::cerr << "[Error] Failed to open file: " << filepath << "!" << std::endl;
      }

      std::vector<glm::vec3> vertices;
      std::vector<glm::vec2> uvCoords;
      std::vector<glm::vec3> normals;
      std::vector<unsigned int> indices;

      std::map<std::string, unsigned int> indexMap;

      // buffered file reading
      std::vector<std::string> data;
      
      for (std::string line; std::getline(input, line);) 
      {
        splitString(line, data, ' ');
        
        if (data[0] == "v") {
          vertices.push_back(glm::vec3(
            std::stof(data[1]),
            std::stof(data[2]),
            std::stof(data[3])
          ));
        } else if (data[0] == "vt") {
          uvCoords.push_back(glm::vec2(
            std::stof(data[1]),
            std::stof(data[2])
          ));
        } else if (data[0] == "vn") {
          normals.push_back(glm::vec3(
            std::stof(data[1]),
            std::stof(data[2]),
            std::stof(data[3])
          ));
        } else if (data[0] == "f") {
          for (int i=1; i < 4; i++) {
            if (indexMap.contains(data[i])) {
              indices.push_back(indexMap[data[i]]);
            } else {
              indices.push_back(indexMap.size());
              indexMap[data[i]] = indexMap.size();
            }
          }
        }
      }

      size_t vertexCount = (unsigned int) indexMap.size();
      float* vertexData = new float[vertexCount * 3];
      float* uvData     = new float[vertexCount * 2];
      float* normalData = new float[vertexCount * 3];

      int ii, vi, ui, ni;
      for (std::pair<std::string, unsigned int> kv : indexMap)
      {
        splitString(kv.first, data, '/');
        ii = kv.second;
        vi = std::stoi(data[0]) - 1;
        ui = std::stoi(data[1]) - 1;
        ni = std::stoi(data[2]) - 1;

        vertexData[ii * 3]     = (vertices[vi].x);
        vertexData[ii * 3 + 1] = (vertices[vi].y);
        vertexData[ii * 3 + 2] = (vertices[vi].z);

        uvData[ii * 2]         = (uvCoords[ui].x);
        uvData[ii * 2 + 1]     = (uvCoords[ui].y);

        normalData[ii * 3]     = (normals[ni].x);
        normalData[ii * 3 + 1] = (normals[ni].y);
        normalData[ii * 3 + 2] = (normals[ni].z);
      }

      gfx::Mesh* mesh = new gfx::Mesh(filepath);
      mesh->setVertexCount(indices.size());
      mesh->createVertexBuffer(0, 3, vertexData, vertexCount);
      mesh->createVertexBuffer(1, 2, uvData, vertexCount);
      mesh->createVertexBuffer(2, 3, normalData, vertexCount);
      mesh->createElementBuffer(indices.data());
      free(vertexData);
      free(uvData);
      free(normalData);

      return mesh;
    }
  }
}