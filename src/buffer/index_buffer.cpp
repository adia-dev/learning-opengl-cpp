#include "index_buffer.h"
#include "renderer/renderer.h"

IndexBuffer::IndexBuffer(const unsigned int *indices, unsigned int count)
    : _count(count) {

  ASSERT(sizeof(unsigned int) == sizeof(GLuint));

  GL_CALL(glGenBuffers(1, &_id));
  GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id));
  GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(unsigned int),
                       indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() { unbind(); }

unsigned int IndexBuffer::ID() const { return _id; }
unsigned int IndexBuffer::count() const { return _count; }
void IndexBuffer::bind() const {
  GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id));
}
void IndexBuffer::unbind() const {
  GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
