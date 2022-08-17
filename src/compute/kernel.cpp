
#include "compute.h"

namespace sunstorm
{
  namespace cmp
  {
    ComputeKernel::ComputeKernel(std::string kernelName, ComputeProgram* program) : name(kernelName), program(program)
    {
      cl_int error;
      kernelId = clCreateKernel(program->getProgram(), kernelName.c_str(), &error);
      ComputeHandler::handleError(error);
      SSRT_DBG_OUTPUT("Created Compute Kernel: " << kernelName);
    }
    
    ComputeKernel::~ComputeKernel()
    {
      ComputeHandler::handleError(clReleaseKernel(kernelId));
      SSRT_DBG_OUTPUT("Destroyed Compute Kernel: " << name);
    }
  }
}