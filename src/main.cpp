
#include <iostream>
#include <chrono>

#include "common.h"
#include "compute/compute.h"
#include "graphics/graphics.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

using namespace sunstorm;

class RayTracer
{
private:
  cmp::ComputeKernel* k;
  unsigned int width;
  unsigned int height;
  cl_mem display;

public:
  RayTracer(cmp::ComputeKernel* kernel, const gfx::Texture& image, unsigned int width, unsigned int height)
    : k(kernel), width(width), height(height)
  {
    display = k->createSharedImage(0, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, image.getTextureId());
    clSetKernelArg(k->getKernel(), 1, sizeof(unsigned int), &width);
    clSetKernelArg(k->getKernel(), 2, sizeof(unsigned int), &height);
  }

  // wrapper method
  static void handleErr(cl_int err) 
  {
    cmp::ComputeHandler::handleError(err);
  }

  void execute(size_t* localSize, size_t* globalSize) const
  {
    cl_command_queue queue = cmp::ComputeHandler::global->getQueue(0);
    handleErr(clEnqueueAcquireGLObjects(queue, 1, &display, 0, NULL, NULL));
    handleErr(clEnqueueNDRangeKernel(queue, k->getKernel(), 2, NULL, globalSize, localSize, 0, NULL, NULL));
    handleErr(clFinish(queue));
    handleErr(clEnqueueReleaseGLObjects(queue, 1, &display, 0, NULL, NULL));
  }
};

void run()
{
  unsigned int w = 512, h = 512;

  /* --- Graphics setup --- */

  gfx::Window window = gfx::Window("Ray Tracer | v0.0.1", w, h);

  gfx::Texture colour = gfx::Texture("Colour Buffer", GL_TEXTURE_2D);
  colour.bind(0);
  colour.storeTexture2D(w, h, 0, nullptr);
  colour.genMipmaps();
  colour.unbind(0);

  gfx::Framebuffer framebuffer = gfx::Framebuffer(w, h);
  framebuffer.bindFramebuffer();
  framebuffer.attachTexture(colour, GL_COLOR_ATTACHMENT0);
  framebuffer.unbindFramebuffer();
  framebuffer.complete();

  /* --- Compute set up --- */
  
  cmp::ComputeHandler handler = cmp::ComputeHandler();
  handler.createQueue(NULL);
  cmp::ComputeProgram* program = handler.createProgram("cl/ray_trace.cl");
  cmp::ComputeKernel* kernel = program->createKernel("trace");
  RayTracer rt = RayTracer(kernel, colour, w, h);

  size_t globalSize[] = { w, h };
  size_t localSize[] = { globalSize[0] / 64, globalSize[1] / 64 };

  /* --- Main Game loop --- */

  while (!window.isClosed())
  {
    window.update();
    // double t0 = glfwGetTime();
    rt.execute(localSize, globalSize);
    framebuffer.draw(window.getWidth(), window.getHeight());
    // SSRT_DBG_OUTPUT("Took: " << (glfwGetTime() - t0) * 1000 << " ms");
  }
}

void run2()
{
  unsigned int w = 512, h = 512;

  /* --- Graphics setup --- */

  gfx::Window window = gfx::Window("Graphics Test | v0.0.1", w, h);

  gfx::Shader shader = gfx::Shader("diffuse");
  shader.createShader(GL_VERTEX_SHADER, io::readFile("glsl/base_vert.glsl"));
  shader.createShader(GL_FRAGMENT_SHADER, io::readFile("glsl/base_frag.glsl"));
  shader.buildProgram();
  shader.unbindProgram();

  gfx::Mesh mesh = *io::readOBJFile("models/cube.obj");

  while (!window.isClosed()) {
    window.update();
    shader.bindProgram();
    mesh.bindMesh();
    glDrawElements(GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_SHORT, 0);
    mesh.unbindMesh();
    shader.unbindProgram();
  }
}

/**
 * Main method - program starts here.
 */
int main(int argc, char const *argv[])
{
  SSRT_DBG_OUTPUT("Program has been initialized successfully!");
  bool success = true;

  try {
    run2();
  } 
  catch(const std::exception& e) {
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
