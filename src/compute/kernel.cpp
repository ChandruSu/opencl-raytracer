
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
      // frees all images and buffers
      for (size_t i = 0; i < memoryObjects.size(); i++) {
        ComputeHandler::handleError(clReleaseMemObject(memoryObjects[i]));
      }
      
      ComputeHandler::handleError(clReleaseKernel(kernelId));
      SSRT_DBG_OUTPUT("Destroyed Compute Kernel: " << name);
    }
    
    cl_mem ComputeKernel::createBuffer(cl_uint index, cl_mem_flags flags, size_t size)
    {
      cl_int error;
      cl_mem memory = clCreateBuffer(ComputeHandler::global->getContext(), flags, size, nullptr, &error);
      ComputeHandler::handleError(error);
      setMemoryArg(index, memory);
      memoryObjects.push_back(memory);
      return memory;
    }
    
    cl_mem ComputeKernel::createSharedBuffer(cl_uint index, cl_mem_flags flags, GLuint bufferId)
    {
      cl_int error;
      cl_mem memory = clCreateFromGLBuffer(ComputeHandler::global->getContext(), flags, bufferId, &error);
      ComputeHandler::handleError(error);
      setMemoryArg(index, memory);
      memoryObjects.push_back(memory);
      return memory;
    }
    
    cl_mem ComputeKernel::createImage(cl_uint index, cl_mem_flags flags, cl_image_format format, cl_image_desc descriptor)
    {
      cl_int error;
      cl_mem image = clCreateImage(ComputeHandler::global->getContext(), flags, &format, &descriptor, nullptr, &error);
      ComputeHandler::handleError(error);
      setMemoryArg(index, image);
      memoryObjects.push_back(image);
      return image;
    }
    
    cl_mem ComputeKernel::createSharedImage(cl_uint index, cl_mem_flags flags, GLenum target, GLint mipLevel, GLuint textureId)
    {
      cl_int error;
      cl_mem image = clCreateFromGLTexture(ComputeHandler::global->getContext(), flags, target, mipLevel, textureId, &error);
      ComputeHandler::handleError(error);
      setMemoryArg(index, image);
      memoryObjects.push_back(image);
      return image;
    }
    
    cl_mem ComputeKernel::createSharedRenderbuffer(cl_uint index, cl_mem_flags flags, GLuint renderBufferId)
    {
      cl_int error;
      cl_mem buf = clCreateFromGLRenderbuffer(ComputeHandler::global->getContext(), flags, renderBufferId, &error);
      ComputeHandler::handleError(error);
      setMemoryArg(index, buf);
      memoryObjects.push_back(buf);
      return buf;
    }
    
    void ComputeKernel::setMemoryArg(cl_uint position, cl_mem memory)
    {
      cl_int error = clSetKernelArg(kernelId, position, sizeof(cl_mem), &memory);
      ComputeHandler::handleError(error);
    }
  }
}