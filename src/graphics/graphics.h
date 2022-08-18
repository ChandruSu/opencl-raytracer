#pragma once

#define GLEW_STATIC

#include <GLEW/glew.h>
#include <GLEW/wglew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <string>

#include "../common.h"

namespace sunstorm
{
  namespace gfx
  {
    class Window
    {
    private:
      GLFWwindow* windowId;
      int width;
      int height;

      /**
       * @brief Configures OpenGL properties and enums for rendering.
       */
      void setup() const;

    public:
      /**
       * @brief Construct a new Window object
       * 
       * @param title Window title
       * @param width Screen width in pixels
       * @param height Screen height in pixels
       */
      Window(std::string title, int width, int height);

      /**
       * @brief Destroy the Window object and terminate GLFW.
       */
      ~Window();

      /**
       * @brief Updates window each frame, swaps images in swapchain and retrieves
       *  user input events.
       */
      void update();

      /**
       * @brief Set the Width object
       * 
       * @param width 
       */
      inline void setWidth(int width) {
        this->width = width;
      }
      
      /**
       * @brief Set the Height object
       * 
       * @param height 
       */
      inline void setHeight(int height) {
        this->height = height;
      }

      /**
       * @brief Get the Aspect Ratio object
       * 
       * @return float width-height aspect ration
       */
      inline float getAspectRatio() const {
        return (float) width / height;
      }

      /**
       * @brief Query if the window will be closed.
       * 
       * @return true If window close has been requested
       * @return false If window is still open
       */
      inline bool isClosed() const {
        return glfwWindowShouldClose(windowId);
      }

      /**
       * @brief Prints OpenGL debug messages to standard error output without
       *    terminating program.
       */
      static void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

      /**
       * @brief Updates window dimension properties when GLFW window is resized.
       */
      static void resizeCallback(GLFWwindow* window, int width, int height);
    };
    
  }
}