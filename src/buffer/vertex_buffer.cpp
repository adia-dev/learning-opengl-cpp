#include "vertex_buffer.h"
#include <renderer/utils.h>

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {

  ASSERT(sizeof(unsigned int) == sizeof(GLuint));

  GL_CALL(glGenBuffers(1, &_id));
  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _id));
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() { unbind(); }

unsigned int VertexBuffer::ID() const { return _id; }
void VertexBuffer::bind() const { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _id)); }
void VertexBuffer::unbind() const { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
