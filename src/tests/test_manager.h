#pragma once

#include <tests/test.h>

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <imgui.h>

namespace test {
class TestManager {
public:
  TestManager() {}
  ~TestManager() {}

  void render();
  void update(float delta_time);

  void imgui_render();

  template <class T, class... TestArgs>
  void register_test(const std::string &name, TestArgs &&...args) {
    std::cout << "Registering test: " << name << std::endl;
    _keys.push_back(name);
    _tests[name] = [&args...]() {
      return std::make_unique<T>(std::forward<TestArgs>(args)...);
    };
  }
  void select_test(const std::string &name);
  void reset_test();
  std::vector<std::string> keys() const;

private:
  std::unordered_map<std::string, std::function<std::unique_ptr<Test>()>>
      _tests;
  std::unique_ptr<Test> _current_test;
  std::string _current_test_name;
  std::vector<std::string> _keys;
};
} // namespace test
