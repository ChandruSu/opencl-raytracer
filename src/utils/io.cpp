#include "utils.h"

namespace sunstorm
{
  namespace io 
  {
    std::string readFile(std::string filepath)
    {
      std::ifstream input(RES_DIR + filepath);

      if (input.fail() || !input.is_open()) {
        std::cerr << "Failed to open file: " << filepath << "!" << std::endl;
      }

      std::stringstream *buf = new std::stringstream();
      (*buf) << input.rdbuf();
      
      return buf->str();
    }
  }
}