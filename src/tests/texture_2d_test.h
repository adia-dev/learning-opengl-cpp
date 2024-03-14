#pragma once

#include "buffer/index_buffer.h"
#include "buffer/vertex_array.h"
#include "buffer/vertex_buffer.h"
#include "renderer/renderer.h"
#include "shader/shader.h"
#include "test.h"
#include "texture/texture_2d.h"
#include <string>

namespace test {
class Texture2DTest : public Test {
public:
  Texture2DTest();
  ~Texture2DTest();

  void on_update(float delta_time) override;
  void on_render() override;
  void on_imgui_render() override;
  void on_reset() override;

private:
  Renderer _renderer;
  Shader _shader;

  std::vector<glm::vec3> _translations;

  std::unique_ptr<Texture2D> _texture_2D;
  std::unique_ptr<VertexArray> _vertex_array;
  std::unique_ptr<VertexBuffer> _vertex_buffer;
  std::unique_ptr<IndexBuffer> _index_buffer;

  char _texture_path_buf[512];
  std::string _last_valid_path;
  bool _local_textures = true;
};
} // namespace test
