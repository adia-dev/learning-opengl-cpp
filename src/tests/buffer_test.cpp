#include "buffer_test.h"
#include "buffer/vertex_buffer_layout.h"
#include "utils/R.h"

namespace test {
BufferTest::BufferTest() {
  const float vertices[] = {
      // positions       // texture coords
      0.5f,  0.5f,  // top right
      0.5f,  -0.5f, // bottom right
      -0.5f, -0.5f, // bottom left
      -0.5f, 0.5f,  // top left
  };

  const unsigned int indices[] = {
      0, 1, 2, // 0
      2, 3, 0  // 1
  };

  _vertex_buffer =
      std::make_unique<VertexBuffer>(vertices, 4 * 2 * sizeof(float));
  _vertex_array = std::make_unique<VertexArray>();

  VertexBufferLayout layout;
  layout.push<float>(2);
  _vertex_array->add_buffer(*_vertex_buffer, layout);

  _index_buffer = std::make_unique<IndexBuffer>(indices, 6);

  _shader
      .add_shader(R::shaders("test/vertex.vert"), ShaderType::Vertex) //
      .add_shader(R::shaders("test/fragment.frag"), ShaderType::Fragment)
      .compile_and_link();

  _vertex_array->unbind();
  _vertex_buffer->unbind();
  _index_buffer->unbind();
  _shader.unbind();
}

BufferTest::~BufferTest() {}
void BufferTest::on_update(float delta_time) {}
void BufferTest::on_render() {
  _renderer.draw(*_vertex_array, *_index_buffer, _shader);
}
void BufferTest::on_imgui_render() {}
} // namespace test
