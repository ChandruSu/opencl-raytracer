
#include <iostream>
#include <chrono>

#include "common.h"
#include "compute/compute.h"
#include "graphics/graphics.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

using namespace sunstorm;

float vertices[] = {
  -1.0,  1.0, 0.0,
   1.0,  1.0, 0.0,
  -1.0, -1.0, 0.0,
   1.0, -1.0, 0.0,
};

float uvCoords[] = {
  0.0, 1.0,
  1.0, 1.0,
  0.0, 0.0,
  1.0, 0.0,
};

unsigned short indices[] = {
  0, 2, 1,
  1, 2, 3,
};

class DrawScreen
{
  private:
  cmp::ComputeKernel* k;
  cl_mem m_image;
  unsigned int width;
  unsigned int height;

  public:
    DrawScreen(cmp::ComputeKernel* kernel, GLuint textureId, unsigned int width, unsigned int height) : k(kernel), width(width), height(height)
    {
      m_image = k->createSharedImage(0, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, textureId);
      clSetKernelArg(k->getKernel(), 1, sizeof(unsigned int), &width);
      clSetKernelArg(k->getKernel(), 2, sizeof(unsigned int), &height);
    }
    
    void execute(float time, size_t* localSize, size_t* globalSize)
    {
      cl_int error = CL_SUCCESS;
      cl_command_queue queue = cmp::ComputeHandler::global->getQueue(0);
      clSetKernelArg(k->getKernel(), 3, sizeof(float), &time);
      
      error = clEnqueueAcquireGLObjects(queue, 1, &m_image, 0, NULL, NULL);
      cmp::ComputeHandler::handleError(error);

      error  = clEnqueueNDRangeKernel(queue, k->getKernel(), 2, NULL, globalSize, localSize, 0, NULL, NULL);
      error |= clFinish(queue);
      cmp::ComputeHandler::handleError(error);

      error = clEnqueueReleaseGLObjects(queue, 1, &m_image, 0, NULL, NULL);
      cmp::ComputeHandler::handleError(error);
    }
};

void run() 
{
  const int w = 512, h = 512;
  float time = 0.0f;

  // -- Setup Graphics -- //
  
  gfx::Window window("Ray Tracer Test", w, h);

  gfx::Shader shader = gfx::Shader("Basic");
  shader.createShader(GL_VERTEX_SHADER, io::readFile("glsl/base_vert.glsl"));
  shader.createShader(GL_FRAGMENT_SHADER, io::readFile("glsl/base_frag.glsl"));
  shader.buildProgram();
  shader.getUniform("tex");

  // creates empty image for compute target

  gfx::Texture img = gfx::Texture("frame", GL_TEXTURE_2D);
  img.bind(0);
  img.storeTexture2D(w, h, 0, nullptr);
  img.genMipmaps();
  img.unbind(0);
  
  // -- Set up Compute -- //
  
  cmp::ComputeHandler handler = cmp::ComputeHandler();
  handler.createQueue(NULL);
  cmp::ComputeProgram* program = handler.createProgram("cl/drawScreen.cl");
  cmp::ComputeKernel* kernel = program->createKernel("draw");

  // --- Compute Test --- //

  DrawScreen draw = DrawScreen(kernel, img.getTextureId(), w, h);
  size_t globalSize[] = { w, h };
  size_t localSize[] = { globalSize[0] / 64, globalSize[1] / 64 };
  draw.execute(time, localSize, globalSize);

  // --- Display Quad --- //

  gfx::Mesh mesh = gfx::Mesh("quad");
  mesh.setVertexCount(6);
  mesh.createVertexBuffer(0, 3, vertices, 4);
  mesh.createVertexBuffer(1, 2, uvCoords, 4);
  mesh.createElementBuffer(indices);

  // -- Main game loop -- //
  while (!window.isClosed()) 
  {
    window.update();
    time = (float) glfwGetTime();
    draw.execute(time, localSize, globalSize);

    shader.bindProgram();
    shader.setUniformTexture("tex", 0);
    img.bind(0);

    mesh.bindMesh();
    glDrawElements(GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_SHORT, 0);
    mesh.unbindMesh();
    
    img.unbind(0);
    shader.unbindProgram();
  }
}

void run2()
{
  int w = 512, h = 512;
  
  // -- Setup Graphics -- //

  gfx::Window window("Ray Tracer Test", w, h);

  gfx::Shader shader = gfx::Shader("Basic");
  shader.createShader(GL_VERTEX_SHADER, io::readFile("glsl/base_vert.glsl"));
  shader.createShader(GL_FRAGMENT_SHADER, io::readFile("glsl/base_frag.glsl"));
  shader.buildProgram();
  shader.getUniform("tex");
  shader.getUniform("projection");

  gfx::Shader post = gfx::Shader("Post Processing");
  post.createShader(GL_VERTEX_SHADER, io::readFile("glsl/postproc_vert.glsl"));
  post.createShader(GL_FRAGMENT_SHADER, io::readFile("glsl/postproc_frag.glsl"));
  post.buildProgram();
  post.getUniform("tex");
  post.getUniform("dep");

  // --- Display Quad --- //

  gfx::Mesh mesh = gfx::Mesh("quad");
  mesh.setVertexCount(6);
  mesh.createVertexBuffer(0, 3, vertices, 4);
  mesh.createVertexBuffer(1, 2, uvCoords, 4);
  mesh.createElementBuffer(indices);

  gfx::Texture texture = *io::readTextureFile("img/texture.jpg");

  // --- Test Mesh --- //

  float t_vertices[] = {
    -1.0, -1.0, -4.0,
     1.0, -1.0, -3.0,
     0.0,  1.0, -2.0,
  };

  float t_uvCoords[] = {
    0.0, 0.0,
    1.0, 0.0,
    0.5, 1.0,
  };

  unsigned short t_indices[] = {
    0, 1, 2
  };

  gfx::Mesh triangle = gfx::Mesh("tri");
  triangle.setVertexCount(3);
  triangle.createVertexBuffer(0, 3, t_vertices, 3);
  triangle.createVertexBuffer(1, 2, t_uvCoords, 3);
  triangle.createElementBuffer(t_indices);

  // --- Framebuffers --- //

  gfx::Texture colour = gfx::Texture("colour", GL_TEXTURE_2D);
  colour.bind(0);
  colour.storeTexture2D(w, h, 0, nullptr);
  colour.genMipmaps();
  colour.unbind(0);

  gfx::Texture depth = gfx::Texture("depth", GL_TEXTURE_2D);
  depth.bind(0);
  depth.storeTexture2D(GL_DEPTH_STENCIL, w, h, 0, nullptr, GL_UNSIGNED_INT_24_8, GL_DEPTH24_STENCIL8);
  depth.genMipmaps();
  depth.unbind(0);

  gfx::Framebuffer fbo = gfx::Framebuffer(w, h);
  fbo.bindFramebuffer();
  fbo.attachTexture(colour, GL_COLOR_ATTACHMENT0);
  fbo.attachTexture(depth, GL_DEPTH_STENCIL_ATTACHMENT);
  fbo.unbindFramebuffer();
  fbo.complete();

  // -- Main game loop -- //
  
  glm::mat4 proj = glm::perspective(glm::radians(60.0f), window.getAspectRatio(), 0.001f, 1000.0f);

  while (!window.isClosed()) 
  {
    window.update();

    // --- draw preprocessed

    fbo.bindFramebuffer();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    shader.bindProgram();
    shader.setUniformMatrix4x4("projection", proj);
    shader.setUniformTexture("tex", 0);
    texture.bind(0);
    triangle.bindMesh();
    glDrawElements(GL_TRIANGLES, triangle.getVertexCount(), GL_UNSIGNED_SHORT, 0);
    triangle.unbindMesh();
    shader.unbindProgram();
    fbo.unbindFramebuffer();

    // --- draw final

    post.bindProgram();
    post.setUniformTexture("tex", 0);
    post.setUniformTexture("dep", 1);
    colour.bind(0);
    depth.bind(1);
    mesh.bindMesh();
    glDrawElements(GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_SHORT, 0);
    mesh.unbindMesh();
    depth.unbind(1);
    colour.unbind(0);
    post.unbindProgram();

    // glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo.getFramebufferId());
    // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // glDrawBuffer(GL_BACK);
    // glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    // glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  }
}

void run3()
{
  const int w = 512, h = 512;

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

  while (!window.isClosed())
  {
    window.update();
    framebuffer.draw(window.getWidth(), window.getHeight());
  }
}

/**
 * @brief Main method - program starts here.
 */
int main(int argc, char const *argv[])
{
  SSRT_DBG_OUTPUT("Program has been initialized successfully!");
  bool success = true;

  try {
    run2();
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
