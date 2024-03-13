#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class R {
public:
  static std::string shaders(const std::string &name) {
    return get_resource_path("shaders", name);
  }

  static std::string images(const std::string &name) {
    return get_resource_path("images", name);
  }

  static std::string textures(const std::string &name) {
    return get_resource_path("textures", name);
  }

private:
  static std::string get_resource_path(const std::string &type,
                                       const std::string &name) {
    std::string path = std::string(RESOURCES_PATH) + type + "/" + name;
    if (!fs::exists(path)) {
      throw std::runtime_error("Resource does not exist: " + path);
    }
    return path;
  }
};
