
#include <iostream>
#include <stdexcept>
#include <vector>

#define GLEW_STATIC

#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <GLEW/glew.h>
#include <GLEW/wglew.h>

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

    class ComputeHandler
    {
    private:
      cl_platform_id platformId;
      cl_device_id deviceId;
      cl_context context;

      std::vector<cl_command_queue> queues;

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
       * @brief Decodes OpenCL error code and throws error.
       * 
       * @param errorId CL error code
       */
      void handleError(cl_int errorId) const;

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
    };
  }
}