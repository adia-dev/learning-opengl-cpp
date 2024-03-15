#pragma once

#include <buffer/vertex_buffer.h>

class VertexBufferLayout;

class VertexArray {
public:
  VertexArray();
  ~VertexArray();

  void add_buffer(const VertexBuffer &vertex_buffer,
                  const VertexBufferLayout &layout);

  void bind() const;
  void unbind() const;

  inline unsigned int ID() const;

private:
  unsigned int _id;
};
