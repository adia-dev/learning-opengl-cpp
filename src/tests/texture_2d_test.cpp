#include "texture_2d_test.h"
#include "buffer/vertex_buffer_layout.h"
#include "imgui.h"
#include "utils/R.h"
#include <config/constants.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <renderer/utils.h>
#include <string>

namespace fs = std::filesystem;

namespace test {
Texture2DTest::Texture2DTest() {

  const float vertices[] = {
      // positions       // texture coords
      50.0f,  50.0f,  0.0f, 1.0f, // top right
      50.0f,  -50.0f, 0.0f, 0.0f, // bottom right
      -50.0f, -50.0f, 1.0f, 0.0f, // bottom left
      -50.0f, 50.0f,  1.0f, 1.0f  // top left
  };

  const unsigned int indices[] = {
      0, 1, 2, // 0
      2, 3, 0  // 1
  };

  _vertex_buffer =
      std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float));
  _vertex_array = std::make_unique<VertexArray>();

  VertexBufferLayout layout;
  layout.push<float>(2);
  layout.push<float>(2);
  _vertex_array->add_buffer(*_vertex_buffer, layout);

  _index_buffer = std::make_unique<IndexBuffer>(indices, 6);

  _shader
      .add_shader(R::shaders("default/vertex.vert"), ShaderType::Vertex) //
      .add_shader(R::shaders("default/fragment.frag"), ShaderType::Fragment)
      .compile_and_link();

  _texture_2D = std::make_unique<Texture2D>();

  _translations.push_back({200.0f, 200.0f, 0.0f});
  _translations.push_back({400.0f, 200.0f, 0.0f});

  _vertex_array->unbind();
  _vertex_buffer->unbind();
  _index_buffer->unbind();
  _shader.unbind();
}
Texture2DTest::~Texture2DTest() {}
void Texture2DTest::on_update(float delta_time) {}

void Texture2DTest::on_render() {
  static glm::mat4 proj = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f,
                                     (float)WINDOW_HEIGHT, -1.0f, 1.0f);
  static glm::mat4 view =
      glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));

  for (auto &translation : _translations) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 mvp = proj * view * model;

    _shader.bind();
    _shader.set_uniform("u_MVP", mvp);
    _renderer.draw(*_vertex_array, *_index_buffer, _shader);
  }
}
void Texture2DTest::on_imgui_render() {
  if (ImGui::Checkbox("Local", &_local_textures)) {
  }
  if (ImGui::InputTextWithHint("##texture_2d_path",
                               "Path to the file of your texture",
                               _texture_path_buf, 512)) {
    if (_local_textures) {
      if (R::exists(ResourceType::Textures, _texture_path_buf)) {
        _last_valid_path = R::textures(_texture_path_buf);

        if (_texture_2D) {
          _texture_2D->set_file_path(_last_valid_path);
          _texture_2D->load();
          _texture_2D->bind();
          _shader.set_uniform("u_Texture", 0);
        }
      }
    } else {
      if (fs::exists(_texture_path_buf)) {
        _last_valid_path = _texture_path_buf;

        if (_texture_2D) {
          _texture_2D->set_file_path(_last_valid_path);
          _texture_2D->load();
          _texture_2D->bind();
          _shader.set_uniform("u_Texture", 0);
        }
      }
    }
  }

  ImGui::SameLine();
  if (ImGui::Button("Load")) {
    if (_texture_2D) {
      _texture_2D->load();
      _texture_2D->bind();
      _shader.set_uniform("u_Texture", 0);
    }
  }
  if (!_last_valid_path.empty()) {
    ImGui::TextWrapped("Current texture: %s", _last_valid_path.c_str());
  }

  static bool show_window = 0;

  ImGui::Begin("Transforms", &show_window);
  for (size_t i = 0; i < _translations.size(); ++i) {
    const std::string id = "##translation_" + std::to_string(i);
    ImGui::SliderFloat3(id.c_str(), &_translations[i][0], 0.0f,
                        (float)WINDOW_WIDTH);
  }
  ImGui::End();
}
void Texture2DTest::on_reset() {}
} // namespace test
