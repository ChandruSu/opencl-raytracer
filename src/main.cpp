
#include <iostream>
#include <chrono>

#include "common.h"
#include "compute/compute.h"
#include "graphics/graphics.h"

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
  img.storeTexture2D(GL_RGBA, w, h, 0, nullptr, GL_RGBA8);
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
    time = glfwGetTime();
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

  gfx::Shader post = gfx::Shader("Post Processing");
  post.createShader(GL_VERTEX_SHADER, io::readFile("glsl/postproc_vert.glsl"));
  post.createShader(GL_FRAGMENT_SHADER, io::readFile("glsl/postproc_frag.glsl"));
  post.buildProgram();
  post.getUniform("tex");

  // --- Display Quad --- //

  gfx::Mesh mesh = gfx::Mesh("quad");
  mesh.setVertexCount(6);
  mesh.createVertexBuffer(0, 3, vertices, 4);
  mesh.createVertexBuffer(1, 2, uvCoords, 4);
  mesh.createElementBuffer(indices);

  gfx::Texture texture = *io::readTextureFile("img/texture.jpg");

  // --- Test Mesh --- //

  float t_vertices[] = {
    -1.0, -1.0, 0.0,
     1.0, -1.0, 0.0,
     0.0, 1.0, 0.0,
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
  colour.storeTexture2D(GL_RGB, w, h, 0, nullptr, GL_RGBA8);
  colour.genMipmaps();
  colour.unbind(0);

  gfx::Renderbuffer rb = gfx::Renderbuffer(w, h, GL_DEPTH24_STENCIL8);
  rb.bindRenderbuffer();
  rb.unbindRenderbuffer();

  gfx::Framebuffer fbo = gfx::Framebuffer();
  fbo.bindFramebuffer();
  fbo.attachTexture(&colour, 0);
  fbo.attachRenderbuffer(&rb, GL_DEPTH_STENCIL_ATTACHMENT);
  fbo.unbindFramebuffer();
  fbo.complete();

  // -- Main game loop -- //
  
  while (!window.isClosed()) 
  {
    window.update();

    // draw preprocessed

    fbo.bindFramebuffer();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    shader.bindProgram();
    shader.setUniformTexture("tex", 0);
    texture.bind(0);

    triangle.bindMesh();
    glDrawElements(GL_TRIANGLES, triangle.getVertexCount(), GL_UNSIGNED_SHORT, 0);
    triangle.unbindMesh();

    shader.unbindProgram();
    fbo.unbindFramebuffer();

    // draw final

    post.bindProgram();
    post.setUniformTexture("tex", 0);
    colour.bind(0);
    
    mesh.bindMesh();
    glDrawElements(GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_SHORT, 0);
    mesh.unbindMesh();
    
    colour.unbind(0);
    post.unbindProgram();
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
