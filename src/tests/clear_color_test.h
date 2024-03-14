#pragma once

#include "test.h"

namespace test {
class ClearColorTest : public Test {
public:
  ClearColorTest();
  ~ClearColorTest();

  void on_update(float delta_time) override;
  void on_render() override;
  void on_imgui_render() override;

private:
  float _clear_color[4];
};
} // namespace test
