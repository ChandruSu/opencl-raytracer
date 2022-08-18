
#include "graphics.h"

namespace sunstorm
{
  namespace gfx
  {
    Shader::Shader(std::string name) : name(name)
    {
      programId = glCreateProgram();
      SSRT_DBG_OUTPUT("Created Shader program: " << name);
    }
    
    Shader::~Shader()
    {
      for (size_t i = 0; i < shaders.size(); i++) {
        glDetachShader(programId, shaders[i]);
        glDeleteShader(shaders[i]);
      }
      
      glDeleteProgram(programId);
      SSRT_DBG_OUTPUT("Destroyed Shader program: " << name);
    }

    void Shader::createShader(GLenum shaderType, std::string source)
    {
      GLint length = (GLint) source.length();
      const char* glsl = source.c_str();

      GLuint shaderId = glCreateShader(shaderType);
      glShaderSource(shaderId, 1, &glsl, &length);
      glCompileShader(shaderId);

      GLint status;
      glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);

      // Checks if shader compiles and prints error log.
      if (status == GL_FALSE) {
        GLint length;
	      glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
	      std::vector<char> errorLog(length);
	      glGetShaderInfoLog(shaderId, length, &length, &errorLog[0]);
        
        throw std::runtime_error("Failed to compile shader for:" + name + "\n" + &errorLog[0]);
      }

      glAttachShader(programId, shaderId);
      shaders.push_back(shaderId);
    }

    void Shader::buildProgram() const
    {
      glLinkProgram(programId);

      // Links program and prints error log if failure
      GLint linkStatus;
      glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
      if (linkStatus == GL_FALSE) {
        GLint length = 0;
	      glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &linkStatus);
	      std::vector<char> errorLog(length);
	      glGetProgramInfoLog(programId, length, &length, &errorLog[0]);
        
        throw std::runtime_error("Failed to link shader program:" + name + "\n" + &errorLog[0]);
      }

      // Validates program and prints error log if failure
      glValidateProgram(programId);

      GLint validateStatus;
      glGetProgramiv(programId, GL_VALIDATE_STATUS, &validateStatus);
      if (validateStatus == GL_FALSE) {
        GLint length = 0;
	      glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &validateStatus);
	      std::vector<char> errorLog(length);
	      glGetProgramInfoLog(programId, length, &length, &errorLog[0]);
        
        throw std::runtime_error("Failed to validate shader program:" + name + "\n" + &errorLog[0]);
      }
    }
    
    void Shader::bindProgram() const
    {
      glUseProgram(programId);
    }

    void Shader::unbindProgram() const
    {
      glUseProgram(0);
    }
    
    void Shader::getUniform(std::string name)
    {
      GLuint location = glGetUniformLocation(programId, name.c_str());
      if (location >= 0) {
        uniformLocations[name] = location;
      } else {
        throw std::runtime_error("Failed to find uniform: " + name + "!");
      }
    }

    GLuint Shader::getUniformLocation(std::string name)
    {
      if (uniformLocations.contains(name)) {
        return uniformLocations[name];
      } else {
        throw std::runtime_error("Uniform not loaded: " + name + "!");
      }
    }
    
    void Shader::setUniformInt(std::string name, int i)
    {
      glUniform1i(getUniformLocation(name), i);
    }
    
    void Shader::setUniformFloat(std::string name, float f)
    {
      glUniform1f(getUniformLocation(name), f);
    }
    
    void Shader::setUniformVector2(std::string name, glm::vec2 v)
    {
      glUniform2f(getUniformLocation(name), v.x, v.y);
    }

    void Shader::setUniformVector3(std::string name, glm::vec3 v)
    {
      glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
    }

    void Shader::setUniformVector4(std::string name, glm::vec4 v)
    {
      glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
    }
    
    void Shader::setUniformMatrix4x4(std::string name, glm::mat4 m)
    {
      glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
    }
  }
}
