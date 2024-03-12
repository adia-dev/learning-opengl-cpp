#include "vertex_array.h"
#include "vertex_buffer_layout.h"
#include <renderer/utils.h>

VertexArray::VertexArray() { GL_CALL(glGenVertexArrays(1, &_id)); }

VertexArray::~VertexArray() { GL_CALL(glDeleteVertexArrays(1, &_id)); }

void VertexArray::add_buffer(const VertexBuffer &vertex_buffer,
                             const VertexBufferLayout &layout) {
  bind();
  vertex_buffer.bind();
  const auto &elements = layout.elements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); ++i) {
    const auto &element = elements[i];
    GL_CALL(glEnableVertexAttribArray(i));
    GL_CALL(glVertexAttribPointer(i, element.count, element.type,
                                  element.normalized, layout.stride(),
                                  (const void *)offset));
    offset +=
        element.count + VertexBufferLayoutElement::size_of_type(element.type);
  }
}

void VertexArray::bind() const { GL_CALL(glBindVertexArray(_id)); }
void VertexArray::unbind() const { GL_CALL(glBindVertexArray(0)); }
