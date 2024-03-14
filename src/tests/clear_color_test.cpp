#include "clear_color_test.h"
#include "imgui.h"
#include <renderer/utils.h>

namespace test {
ClearColorTest::ClearColorTest() : _clear_color{0.5f, 0.2f, 0.75f, 1.0f} {}
ClearColorTest::~ClearColorTest() {}
void ClearColorTest::on_update(float delta_time) {}
void ClearColorTest::on_render() {
  GL_CALL(glClearColor(_clear_color[0], _clear_color[1], _clear_color[2],
                       _clear_color[3]));
  GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}
void ClearColorTest::on_imgui_render() {
  ImGui::ColorEdit4("Clear Color", _clear_color);
}
void ClearColorTest::on_reset() {
  _clear_color[0] = 0.5f;
  _clear_color[1] = 0.2f;
  _clear_color[2] = 0.75f;
  _clear_color[3] = 1.0f;
}
} // namespace test
