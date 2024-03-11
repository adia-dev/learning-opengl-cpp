#pragma once

#include <renderer/renderer.h>
#include <vector>

struct VertexBufferLayoutElement {
  unsigned int type;
  unsigned int count;
  unsigned int normalized;

  static unsigned int size_of_type(unsigned int type) {
    switch (type) {
    case GL_FLOAT:
      return 4;
    case GL_UNSIGNED_INT:
      return 4;
    case GL_UNSIGNED_BYTE:
      return 1;
    }

    ASSERT(false);
    return 0;
  }
};

class VertexBufferLayout {
public:
  VertexBufferLayout() : _stride(0) {}

  template <typename T> void push(unsigned int count) { ASSERT(false); }

  template <> void push<unsigned int>(unsigned int count) {
    _elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    _stride += count * VertexBufferLayoutElement::size_of_type(GL_UNSIGNED_INT);
  }

  template <> void push<float>(unsigned int count) {
    _elements.push_back({GL_FLOAT, count, GL_FALSE});
    _stride += count * VertexBufferLayoutElement::size_of_type(GL_FLOAT);
  }

  template <> void push<unsigned char>(unsigned int count) {
    _elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    _stride +=
        count * VertexBufferLayoutElement::size_of_type(GL_UNSIGNED_BYTE);
  }

  inline const std::vector<VertexBufferLayoutElement> &elements() const {
    return _elements;
  }

  inline unsigned int stride() const { return _stride; }

private:
  std::vector<VertexBufferLayoutElement> _elements;
  unsigned int _stride;
};
