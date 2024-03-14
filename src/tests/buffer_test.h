#pragma once

#include "buffer/index_buffer.h"
#include "buffer/vertex_array.h"
#include "buffer/vertex_buffer.h"
#include "renderer/renderer.h"
#include "shader/shader.h"
#include "test.h"
#include <memory>

namespace test {
class BufferTest : public Test {
public:
  BufferTest();
  ~BufferTest();

  void on_update(float delta_time) override;
  void on_render() override;
  void on_imgui_render() override;

private:
  Renderer _renderer;
  Shader _shader;

  std::unique_ptr<VertexArray> _vertex_array;
  std::unique_ptr<VertexBuffer> _vertex_buffer;
  std::unique_ptr<IndexBuffer> _index_buffer;
};
} // namespace test
