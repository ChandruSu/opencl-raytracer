
#include <iostream>

#include "compute/compute.h"

using namespace sunstorm;

int main(int argc, char const *argv[])
{
  try
  {
    cmp::ComputeHandler handler = cmp::ComputeHandler();
    handler.createQueue(NULL);
    cmp::ComputeProgram* program = handler.createProgram("cl/addVec.cl");
    cmp::ComputeKernel* kernel = program->createKernel("vecAdd");
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  SSRT_DBG_OUTPUT("Program has ended successfully!");
  return 0;
}
