
#include <iostream>

#include "common.h"
#include "compute/compute.h"
#include "graphics/graphics.h"

using namespace sunstorm;

class AddVec
{
  private:
    cmp::ComputeKernel* k;
    cl_mem m_a;
    cl_mem m_b;
    cl_mem m_c;
    const unsigned int n;

  public:
    AddVec(cmp::ComputeKernel* kernel, const unsigned int n) : k(kernel), n(n)
    {
      m_a = k->createBuffer(0, CL_MEM_READ_ONLY, n * sizeof(float));
      m_b = k->createBuffer(1, CL_MEM_READ_ONLY, n * sizeof(float));
      m_c = k->createBuffer(2, CL_MEM_WRITE_ONLY,n * sizeof(float));
      clSetKernelArg(k->getKernel(), 3, sizeof(unsigned int), &n);
    }

    void execute(float* a, float* b, float* c, size_t* localSize, size_t* globalSize)
    {
      cl_int error = CL_SUCCESS;
      cl_command_queue queue = cmp::ComputeHandler::global->getQueue(0);
      error |= clEnqueueWriteBuffer(queue, m_a, CL_TRUE, 0, n * sizeof(float), a, 0, NULL, NULL);
      error |= clEnqueueWriteBuffer(queue, m_b, CL_TRUE, 0, n * sizeof(float), b, 0, NULL, NULL);
      cmp::ComputeHandler::handleError(error);

      error = clEnqueueNDRangeKernel(queue, k->getKernel(), 1, NULL, globalSize, localSize, 0, NULL, NULL);
      cmp::ComputeHandler::handleError(error);
      clFinish(queue);

      error = clEnqueueReadBuffer(queue, m_c, CL_TRUE, 0, n * sizeof(float), c, 0, NULL, NULL);
      cmp::ComputeHandler::handleError(error);
    }
};

void run() 
{
  cmp::ComputeHandler handler = cmp::ComputeHandler();
  handler.createQueue(NULL);
  cmp::ComputeProgram* program = handler.createProgram("cl/addVec.cl");
  cmp::ComputeKernel* kernel = program->createKernel("vecAdd");

  const unsigned int n = 1024;
  AddVec addVec = AddVec(kernel, n);

  float a[n];
  float b[n];
  float c[n];

  size_t localSize = 64;
  size_t globalSize = (size_t)(ceil(n/(float)localSize)*localSize);

  for (size_t i = 0; i < n; i++) {
    a[i] = (float)i;
    b[i] = (float)(n - i);
  }

  addVec.execute(a, b, c, &localSize, &globalSize);

  float sum = 0.0f;
  for (size_t i = 0; i < n; i++) {
    sum += c[i];
  }

  std::cout << "Result = " << sum << ", True = " << (1024.0f * 1024.0f) << std::endl;

  gfx::Window window("Ray Tracer Test", 512, 512);

  while (!window.isClosed()) {
    window.update();
  }
}

int main(int argc, char const *argv[])
{
  bool success = true;
  try
  {
    run();
  }
  catch(const std::exception& e)
  {
    success = false;
    std::cerr << "[Error] " << e.what() << std::endl;
  }

  if (success) {
    SSRT_DBG_OUTPUT("Program has ended successfully!");
  } else {
    SSRT_DBG_OUTPUT("Program has ended prematurely due to errors!");
  }
  return 0;
}
