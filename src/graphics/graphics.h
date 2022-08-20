#pragma once

#define GLEW_STATIC

#include <GLEW/glew.h>
#include <GLEW/wglew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

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
    
    class Shader
    {
    private:
      GLuint programId;
      std::string name;
      std::vector<GLuint> shaders;
      std::map<std::string, GLuint> uniformLocations;

      /**
       * @brief Get the Uniform location of uniform from map.
       * 
       * @param name Name of uniform
       * @return GLuint 
       */
      GLuint getUniformLocation(std::string name);

    public:
      /**
       * @brief Construct a new empty Shader program object.
       */
      Shader(std::string name);

      /**
       * @brief Destroy the Shader program and attached shaders.
       */
      ~Shader();

      /**
       * @brief Builds program by linking subshaders and validating
       *  the shader program.
       */
      void buildProgram() const;

      /**
       * @brief Create, compile and attache sub-shader to shader program.
       * 
       * @param shaderType Shader type i.e vertex, fragment, geometry
       * @param source Source code for shader
       */
      void createShader(GLenum shaderType, std::string source);

      /**
       * @brief Binds this shader program to be used to render meshes.
       */
      void bindProgram() const;

      /**
       * @brief Binds the default program to render meshes.
       */
      void unbindProgram() const;

      /**
       * @brief Get the location of uniform by name and store it in map.
       * 
       * @param name Uniform name
       */
      void getUniform(std::string name);

      /**
       * @brief Set the value of a Uniform Int.
       * 
       * @param name 
       * @param i Int
       */
      void setUniformInt(std::string name, int i);

      /**
       * @brief Set the value of a Uniform Float.
       * 
       * @param name 
       * @param f Float
       */
      void setUniformFloat(std::string name, float f);
      
      /**
       * @brief Set the value of a Uniform Vector2.
       * 
       * @param name 
       * @param v GLM vector 2
       */
      void setUniformVector2(std::string name, glm::vec2 v);
      
      /**
       * @brief Set the value of a Uniform Vector3.
       * 
       * @param name 
       * @param v GLM vector 3
       */
      void setUniformVector3(std::string name, glm::vec3 v);
      
      /**
       * @brief Set the value of a Uniform Vector4.
       * 
       * @param name 
       * @param v GLM vector 4
       */
      void setUniformVector4(std::string name, glm::vec4 v);
      
      /**
       * @brief Set the value of a Uniform Matrix4x4.
       * 
       * @param name 
       * @param m GLM matrix 4x4
       */
      void setUniformMatrix4x4(std::string name, glm::mat4 m);
      
      /**
       * @brief Set the Uniform Texture sampler to active texture location.
       * 
       * @param name
       * @param textureLocation Index of active texture bound
       */
      void setUniformTexture(std::string name, int textureLocation);
    };

    class Mesh
    {
    private:
      GLuint vaoId;
      GLuint elementBufferId;
      int vertexCount;
      std::string name;
      std::vector<GLuint> vbos;

    public:
      /**
       * @brief Construct a new Mesh object and Vertex array object to store
       *    vertex data.
       * 
       * @param name Mesh name
       */
      Mesh(std::string name);

      /**
       * @brief Destroy the Mesh object and delete attached Vertex buffer
       *    objects.
       */
      ~Mesh();

      /**
       * @brief Binds Vertex arrays and element index buffer so that mesh can
       *    be drawn with glDrawElements
       */
      void bindMesh() const;

      /**
       * @brief Unbinds Vertex arrays and uses default element index buffer.
       */
      void unbindMesh() const;

      /**
       * @brief Create a Vertex Buffer object and stores floating point vertex data
       *    and binds it to vertex attribute for shader input.
       * 
       * @param pos Vertex attribute position index
       * @param dim Vertex vector dimensions
       * @param data Float data
       * @param size Number of vertices
       */
      void createVertexBuffer(int pos, int dim, GLfloat* data, GLsizeiptr size);

      /**
       * @brief Create an Element Buffer object.
       * 
       * @param data Element indices
       */
      void createElementBuffer(GLushort* data);

      /**
       * @brief Get the Element Buffer Id
       * 
       * @return GLuint 
       */
      inline GLuint getElementBufferId() const {
        return elementBufferId;
      }
    
      /**
       * @brief Set the Vertex Count
       * 
       * @param vertexCount 
       */
      inline void setVertexCount(int vertexCount) {
        this->vertexCount = vertexCount;
      }

      /**
       * @brief Get the Vertex Count
       * 
       * @return int 
       */
      inline int getVertexCount() const {
        return vertexCount;
      }
    };

    class Texture
    {
    private:
      GLuint textureId;
      GLenum target;
      std::string name;

    public:
      /**
       * @brief Construct a new empty Texture.
       * 
       * @param name texture name
       * @param type texture target type
       */
      Texture(std::string name, GLenum type);
      
      /**
       * @brief Destroy the Texture object from memory.
       */
      ~Texture();

      /**
       * @brief Bind texture to specified texture slot.
       * 
       * @param i Texture slot
       */
      void bind(int i) const;
      
      /**
       * @brief Unbind texture from specified texture slot.
       * 
       * @param i Texture slot
       */
      void unbind(int i) const;

      /**
       * @brief Set OpenGL integer texture parameters.
       * 
       * @param parameterName Parameter name enum
       * @param value Integer parameter value 
       */
      void setTexParamI(GLenum parameterName, GLint value) const;

      /**
       * @brief Generate mip maps using specified mip levels.
       */
      void genMipmaps() const;
      
      void storeTexture2D(int w, int h, int level, unsigned char* image) const;
      
      void storeTexture2D(GLint format, int w, int h, int level, unsigned char* image, GLint type, GLint internalFormat) const;

      /**
       * @brief Get the Texture Id
       * 
       * @return GLuint 
       */
      inline GLuint getTextureId() const {
        return textureId;
      }

      /**
       * @brief Get the Texture Target
       * 
       * @return GLenum 
       */
      inline GLenum getTarget() const {
        return target;
      }
    };

    class Renderbuffer
    {
    private:
      GLuint renderbufferId;
      int width;
      int height;
      GLenum internalFormat;

    public:
      Renderbuffer(int width, int height, GLenum internalFormat);

      ~Renderbuffer();
      
      void bindRenderbuffer() const;

      void unbindRenderbuffer() const;

      inline int getRenderbufferId() const {
        return renderbufferId;
      }

      inline int getWidth() const {
        return width;
      }

      inline int getHeight() const {
        return height;
      }
    };

    class Framebuffer
    {
    private:
      GLuint framebufferId;

    public:
      Framebuffer();

      ~Framebuffer();

      void bindFramebuffer() const;

      void unbindFramebuffer() const;

      void attachTexture(Texture* texture, GLenum attachment) const;

      void attachRenderbuffer(Renderbuffer* renderBuffer, GLenum target) const;

      void complete() const;
    };
  }
}