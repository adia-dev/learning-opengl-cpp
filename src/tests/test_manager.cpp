#include "test_manager.h"

namespace test {

void TestManager::render() {
  if (_current_test == nullptr)
    return;

  _current_test->on_render();
}
void TestManager::imgui_render() {

  static bool opened = true;
  if (ImGui::Begin("Test Params"), &opened) {
    static const char *current_item = NULL;

    if (ImGui::BeginCombo("##select_test_combo", current_item)) {
      for (int n = 0; n < _keys.size(); n++) {
        bool is_selected = (current_item == _keys[n].c_str());

        if (ImGui::Selectable(_keys[n].c_str(), is_selected)) {
          current_item = _keys[n].c_str();
          select_test(current_item);
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    if (_current_test) {
      _current_test->on_imgui_render();
    }

    ImGui::End();
  }
}

void TestManager::update(float delta_time) {
  if (_current_test == nullptr)
    return;

  _current_test->on_update(delta_time);
}
void TestManager::select_test(const std::string &name) {
  if (_tests.find(name) == _tests.end()) {
    std::cout << "No test found with the name: " << name << "\n";
    return;
  }

  _current_test_name = name;
  _current_test = _tests[name]();
}
void TestManager::reset_test() { select_test(_current_test_name); }
std::vector<std::string> TestManager::keys() const { return _keys; }

} // namespace test
