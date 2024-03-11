#pragma once

class IndexBuffer {
public:
  IndexBuffer(const unsigned int *indices, unsigned int count);
  ~IndexBuffer();

  unsigned int ID() const;
  unsigned int count() const;
  void bind() const;
  void unbind() const;

private:
  unsigned int _id;
  unsigned int _count;
};
