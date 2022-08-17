#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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
  }
}