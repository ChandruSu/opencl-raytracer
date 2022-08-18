
#include "graphics.h"

namespace sunstorm
{
  namespace gfx
  {
    Mesh::Mesh(std::string name) : name(name)
    {
      this->elementBufferId = 0;
      glGenVertexArrays(1, &vaoId);
      SSRT_DBG_OUTPUT("Created Mesh: " << name);
    }
    
    Mesh::~Mesh()
    {
      glDeleteBuffers(1, &elementBufferId);
      glDeleteBuffers((GLsizei)vbos.size(), vbos.data());
      glDeleteVertexArrays(1, &vaoId);
      SSRT_DBG_OUTPUT("Destroyed Mesh: " << name);
    }
    
    void Mesh::bindMesh() const
    {
      glBindVertexArray(vaoId);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
    }

    void Mesh::unbindMesh() const
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }

    void Mesh::createVertexBuffer(int pos, int dim, GLfloat* data, GLsizeiptr size)
    {
      bindMesh();
      GLuint bufferId;
      glGenBuffers(1, &bufferId);
      glBindBuffer(GL_ARRAY_BUFFER, bufferId);
      glBufferData(GL_ARRAY_BUFFER, dim * size * sizeof(GLfloat), data, GL_STATIC_DRAW);
      glEnableVertexAttribArray(pos);
      glVertexAttribPointer(pos, dim, GL_FLOAT, GL_FALSE, 0, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      vbos.push_back(bufferId);
      unbindMesh();
    }

    void Mesh::createElementBuffer(GLushort* data)
    {
      bindMesh();
      glGenBuffers(1, &elementBufferId);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount * sizeof(GLushort), data, GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      unbindMesh();
    }
  }
}