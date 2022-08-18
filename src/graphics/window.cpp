#include "graphics.h"

namespace sunstorm
{
  namespace gfx
  {
    Window::Window(std::string title, int width, int height)
      : width(width), height(height)
    {
      if (glfwInit() == GLFW_FALSE) {
        throw std::runtime_error("Failed to initialize GLFW!");
      }

      // Sets up window properties.
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
      glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

      windowId = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

      if (!windowId) {
        throw std::runtime_error("Failed to create GLFW window!");
      }

      // Creates OpenGL rendering context
      glfwMakeContextCurrent(windowId);

      if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW!");
      }

      setup();
      glfwSwapInterval(1);
      glfwSetWindowUserPointer(windowId, this);
      glfwSetWindowSizeCallback(windowId, Window::resizeCallback);
      glfwShowWindow(windowId);

      SSRT_DBG_OUTPUT("Created Window");
    }
    
    Window::~Window()
    {
      glfwTerminate();
      
      SSRT_DBG_OUTPUT("Destroyed Window");
    }

    void Window::setup() const {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_CULL_FACE);
      glEnable(GL_BLEND);
      glFrontFace (GL_CCW);
      glCullFace(GL_BACK);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDebugMessageCallback(Window::debugCallback, nullptr);
    }

    void Window::update()
    {
      glfwPollEvents();
      glfwSwapBuffers(windowId);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      glClearColor(0, 0, 0, 1);
    }
    
    void Window::debugCallback(
      GLenum source, 
      GLenum type, 
      GLuint id, 
      GLenum severity, 
      GLsizei length, 
      const GLchar* message, 
      const void* userParam)
    {
      if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
        std::fprintf(stderr, "GL CALLBACK: id=0x%x, type=0x%x, severity=0x%x, message='%s'\n", id, type, severity, message);
      }
    }
    
    void Window::resizeCallback(GLFWwindow* window, int width, int height)
    {
      Window* win = (Window*)glfwGetWindowUserPointer(window);
      win->setWidth(width);
      win->setHeight(height);
    }
  }
}