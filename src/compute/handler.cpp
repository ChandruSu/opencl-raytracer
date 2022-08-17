#include "compute.h"

namespace sunstorm
{
  namespace cmp
  {
    ComputeHandler* ComputeHandler::global;

    ComputeHandler::ComputeHandler()
    {
      // singleton presence check
      if (global) {
        throw std::runtime_error("Global compute handler instance already exists!");
      }

      handleError(clGetPlatformIDs(1, &platformId, NULL));

      handleError(clGetDeviceIDs(platformId, CL_DEVICE_TYPE_GPU, 1, &deviceId, NULL));

      // creates compute context with GL interoperability 
      cl_context_properties properties[] = {
        CL_GL_CONTEXT_KHR,    (cl_context_properties) wglGetCurrentContext(),
        CL_WGL_HDC_KHR,       (cl_context_properties) wglGetCurrentDC(),
        CL_CONTEXT_PLATFORM,  (cl_context_properties) platformId, 0
      };

      cl_int error;
      context = clCreateContext(NULL, 1, &deviceId, NULL, NULL, &error);
      handleError(error);

      global = this;
      SSRT_DBG_OUTPUT("Created Compute Handler");
    }
    
    ComputeHandler::~ComputeHandler()
    {
      for (size_t i = 0; i < programs.size(); i++) {
        delete programs[i];
      }

      // releases queues
      for (size_t i = 0; i < queues.size(); i++) {
        handleError(clReleaseCommandQueue(queues[i]));
      }
      
      // releases context from memory
      handleError(clReleaseContext(context));

      SSRT_DBG_OUTPUT("Destroyed Compute Handler");
    }
    
    cl_command_queue ComputeHandler::createQueue(cl_command_queue_properties props)
    {
      cl_int error;
      cl_command_queue queue = clCreateCommandQueue(context, deviceId, props, &error);
      handleError(error);
      queues.push_back(queue);
      return queue;
    }
    
    ComputeProgram* ComputeHandler::createProgram(std::string filepath)
    {
      ComputeProgram* program = new ComputeProgram(filepath, io::readFile(filepath));
      programs.push_back(program);
      return program;
    }
    
    void ComputeHandler::handleError(cl_int errorId)
    {
      if (errorId != CL_SUCCESS) {
        throw std::runtime_error(std::string("OpenCL Error: ") + getErrorString(errorId));
      }
    }

    const char* getErrorString(cl_int error) 
    {
      switch(error)
      {
        // run-time and JIT compiler errors
        case 0: return "CL_SUCCESS";
        case -1: return "CL_DEVICE_NOT_FOUND";
        case -2: return "CL_DEVICE_NOT_AVAILABLE";
        case -3: return "CL_COMPILER_NOT_AVAILABLE";
        case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case -5: return "CL_OUT_OF_RESOURCES";
        case -6: return "CL_OUT_OF_HOST_MEMORY";
        case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case -8: return "CL_MEM_COPY_OVERLAP";
        case -9: return "CL_IMAGE_FORMAT_MISMATCH";
        case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case -11: return "CL_BUILD_PROGRAM_FAILURE";
        case -12: return "CL_MAP_FAILURE";
        case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case -15: return "CL_COMPILE_PROGRAM_FAILURE";
        case -16: return "CL_LINKER_NOT_AVAILABLE";
        case -17: return "CL_LINK_PROGRAM_FAILURE";
        case -18: return "CL_DEVICE_PARTITION_FAILED";
        case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

        // compile-time errors
        case -30: return "CL_INVALID_VALUE";
        case -31: return "CL_INVALID_DEVICE_TYPE";
        case -32: return "CL_INVALID_PLATFORM";
        case -33: return "CL_INVALID_DEVICE";
        case -34: return "CL_INVALID_CONTEXT";
        case -35: return "CL_INVALID_QUEUE_PROPERTIES";
        case -36: return "CL_INVALID_COMMAND_QUEUE";
        case -37: return "CL_INVALID_HOST_PTR";
        case -38: return "CL_INVALID_MEM_OBJECT";
        case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case -40: return "CL_INVALID_IMAGE_SIZE";
        case -41: return "CL_INVALID_SAMPLER";
        case -42: return "CL_INVALID_BINARY";
        case -43: return "CL_INVALID_BUILD_OPTIONS";
        case -44: return "CL_INVALID_PROGRAM";
        case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
        case -46: return "CL_INVALID_KERNEL_NAME";
        case -47: return "CL_INVALID_KERNEL_DEFINITION";
        case -48: return "CL_INVALID_KERNEL";
        case -49: return "CL_INVALID_ARG_INDEX";
        case -50: return "CL_INVALID_ARG_VALUE";
        case -51: return "CL_INVALID_ARG_SIZE";
        case -52: return "CL_INVALID_KERNEL_ARGS";
        case -53: return "CL_INVALID_WORK_DIMENSION";
        case -54: return "CL_INVALID_WORK_GROUP_SIZE";
        case -55: return "CL_INVALID_WORK_ITEM_SIZE";
        case -56: return "CL_INVALID_GLOBAL_OFFSET";
        case -57: return "CL_INVALID_EVENT_WAIT_LIST";
        case -58: return "CL_INVALID_EVENT";
        case -59: return "CL_INVALID_OPERATION";
        case -60: return "CL_INVALID_GL_OBJECT";
        case -61: return "CL_INVALID_BUFFER_SIZE";
        case -62: return "CL_INVALID_MIP_LEVEL";
        case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
        case -64: return "CL_INVALID_PROPERTY";
        case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
        case -66: return "CL_INVALID_COMPILER_OPTIONS";
        case -67: return "CL_INVALID_LINKER_OPTIONS";
        case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

        // extension errors
        case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
        case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
        case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
        case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
        case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
        default: return "Unknown OpenCL error";
      }
    }
  }
}