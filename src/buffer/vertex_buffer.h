#pragma once

class VertexBuffer {
public:
  VertexBuffer(const void *data, unsigned int size);
  ~VertexBuffer();

  unsigned int ID() const;
  void bind() const;
  void unbind() const;

private:
  unsigned int _id;
};
