#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#define GLEW_STATIC

#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <GLEW/glew.h>
#include <GLEW/wglew.h>

#include "../common.h"
#include "../utils/utils.h"

namespace sunstorm
{
  namespace cmp
  {
    /**
     * @brief Get the Error String from OpenCL error ID
     * 
     * @param error ID
     * @return error message 
     */
    const char* getErrorString(cl_int error);

    // forward declaration
    class ComputeProgram;
    class ComputeKernel;

    class ComputeHandler
    {
    private:
      cl_platform_id platformId;
      cl_device_id deviceId;
      cl_context context;

      std::vector<cl_command_queue> queues;
      std::vector<ComputeProgram*> programs;

    public:
      static ComputeHandler* global;

      /**
       * @brief Construct a new Compute Handler object and retrieves GPU
       *  details and creates compute context.
       */
      ComputeHandler();

      /**
       * @brief Destroy child objects and releases them from GPU memory.
       */
      ~ComputeHandler();

      /**
       * @brief Create a Queue object and stores it to be released on cleanup.
       * 
       * @param props Command queue properties / Can be NULL
       * @return cl_command_queue 
       */
      cl_command_queue createQueue(cl_command_queue_properties props);

      /**
       * @brief Create a Program object from source file.
       * 
       * @param filepath Path to source file
       * @return ComputeProgram* 
       */
      ComputeProgram* createProgram(std::string filepath);

      /**
       * @brief Get a Command Queue by index
       * 
       * @param i Index in queues
       * @return cl_command_queue 
       */
      inline cl_command_queue getQueue(int i) const {
        return queues[i];
      }

      /**
       * @brief Get the compute Context object.
       * 
       * @return cl_context 
       */
      inline cl_context getContext() const {
        return context;
      }

      /**
       * @brief Get the Device id
       * 
       * @return cl_device_id
       */
      inline cl_device_id getDevice() const {
        return deviceId;
      }
      
      /**
       * @brief Decodes OpenCL error code and throws error.
       * 
       * @param errorId CL error code
       */
      static void handleError(cl_int errorId);
    };

    class ComputeProgram
    {
    private:
      std::string name;
      std::vector<ComputeKernel*> kernels;
      cl_program programId;

    public:
      /**
       * @brief Construct a new Compute Program object to extract kernel
       *  functions from.
       * 
       * @param name Name of program file
       * @param source Source code for program
       */
      ComputeProgram(std::string name, std::string source);

      /**
       * @brief Destroy the Compute Program object and attached kernels.
       */
      ~ComputeProgram();

      /**
       * @brief Builds compute program and prints output log.
       */
      void build() const;

      /**
       * @brief Get the Program object.
       * 
       * @return cl_program 
       */
      inline cl_program getProgram() const {
        return programId;
      }

      /**
       * @brief Create a Kernel object from the program.
       * 
       * @param kernelName Name of kernel function
       * @return ComputeKernel*
       */
      ComputeKernel* createKernel(std::string kernelName);
    };

    class ComputeKernel
    {
    private:
      cl_kernel kernelId;
      std::string name;
      ComputeProgram* program;

    public:
      /**
       * @brief Construct a new Compute Kernel object
       * 
       * @param kernelName Kernel name
       * @param program Program
       */
      ComputeKernel(std::string kernelName, ComputeProgram* program);

      /**
       * @brief Destroy the Compute Kernel object and cleanup attached buffers
       *  and CL memory objects.
       */
      ~ComputeKernel();
    };
  }
}