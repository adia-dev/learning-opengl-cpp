#pragma once

#include <buffer/index_buffer.h>
#include <buffer/vertex_array.h>
#include <shader/shader.h>

class Renderer {
public:
  Renderer() = default;
  ~Renderer() = default;

  void draw(const VertexArray &vertex_array, const IndexBuffer &index_buffer,
            const Shader &shader) const;
  void clear() const;

private:
};
