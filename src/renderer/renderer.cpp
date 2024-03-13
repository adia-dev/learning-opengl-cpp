#include "renderer.h"
#include <renderer/utils.h>

void Renderer::draw(const VertexArray &vertex_array,
                    const IndexBuffer &index_buffer,
                    const Shader &shader) const {

  shader.bind();
  vertex_array.bind();
  index_buffer.bind();

  GL_CALL(glDrawElements(GL_TRIANGLES, index_buffer.count(), GL_UNSIGNED_INT,
                         nullptr));
}

void Renderer::clear() const {

  GL_CALL(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
  GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}
