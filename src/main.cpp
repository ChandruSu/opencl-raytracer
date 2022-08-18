
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
  // Sets up graphics environment
  gfx::Window window("Ray Tracer Test", 512, 512);

  gfx::Shader shader = gfx::Shader("Basic");
  shader.createShader(GL_VERTEX_SHADER, io::readFile("glsl/vert.glsl"));
  shader.createShader(GL_FRAGMENT_SHADER, io::readFile("glsl/frag.glsl"));
  shader.buildProgram();

  // Sets up compute environment
  cmp::ComputeHandler handler = cmp::ComputeHandler();
  handler.createQueue(NULL);
  cmp::ComputeProgram* program = handler.createProgram("cl/addVec.cl");
  cmp::ComputeKernel* kernel = program->createKernel("vecAdd");

  // --- Compute Test --- //
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

  std::cout << "[Output] Result = " << sum << ", True = " << (1024.0f * 1024.0f) << std::endl;
  // -------------------- //

  // --- Display Quad --- //
  float vertices[] = {
    -1.0,  1.0, 0.0,
     1.0,  1.0, 0.0,
    -1.0, -1.0, 0.0,
     1.0, -1.0, 0.0,
  };

  float uvCoords[] = {
    0.0, 0.0,
    0.0, 1.0,
    1.0, 0.0,
    1.0, 1.0,
  };

  unsigned short indices[] = {
    0, 2, 1,
    1, 2, 3,
  };

  gfx::Mesh mesh = gfx::Mesh("quad");
  mesh.setVertexCount(6);
  mesh.createVertexBuffer(0, 3, vertices, 4);
  mesh.createVertexBuffer(1, 2, uvCoords, 4);
  mesh.createElementBuffer(indices);
  // -------------------- //

  // -- Main game loop -- //
  while (!window.isClosed()) {
    window.update();
    shader.bindProgram();
    mesh.bindMesh();
    glDrawElements(GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_SHORT, 0);
    mesh.unbindMesh();
    shader.unbindProgram();
  }
}

int main(int argc, char const *argv[])
{
  SSRT_DBG_OUTPUT("Program has been initialized successfully!");
  bool success = true;

  try {
    run();
  } catch(const std::exception& e) {
    std::cerr << "[Error] " << e.what() << std::endl;
    success = false;
  }

  if (success) {
    SSRT_DBG_OUTPUT("Program has ended successfully!");
  } else {
    SSRT_DBG_OUTPUT("Program has ended prematurely due to errors!");
  }
  return 0;
}
