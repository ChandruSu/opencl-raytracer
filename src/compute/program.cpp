
#include "compute.h"

namespace sunstorm
{
  namespace cmp
  {
    ComputeProgram::ComputeProgram(std::string name, std::string source) : name(name)
    {
      cl_int error;
      const char* cSource = source.c_str();
      programId = clCreateProgramWithSource(ComputeHandler::global->getContext(), 1, &cSource, NULL, &error);
      ComputeHandler::handleError(error);
      build();
      SSRT_DBG_OUTPUT("Created Compute Program: " << name);
    }

    void ComputeProgram::build() const {
      cl_int error = clBuildProgram(programId, 0, NULL, NULL, NULL, NULL);

      // prints build info log on failure.
      if (error != CL_SUCCESS) {
        char log[10000];
        clGetProgramBuildInfo(programId, ComputeHandler::global->getDevice(), CL_PROGRAM_BUILD_LOG, 10000 * sizeof(char), log, NULL);
        std::cerr << "Failed to compile compute program (" << std::string(name) <<")" << std::endl << std::string(log) << std::endl;
      }
      
      ComputeHandler::handleError(error);
    }
    
    ComputeProgram::~ComputeProgram()
    {
      ComputeHandler::handleError(clReleaseProgram(programId));
      SSRT_DBG_OUTPUT("Destroyed Compute Program: " << name);
    }
  }
}