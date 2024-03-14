#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

enum class ResourceType {
  Shaders,
  Images,
  Textures,
};

class R {
public:
  static std::string shaders(const std::string &name) {
    return get_resource_path(ResourceType::Shaders, name);
  }

  static std::string images(const std::string &name) {
    return get_resource_path(ResourceType::Images, name);
  }

  static std::string textures(const std::string &name) {
    return get_resource_path(ResourceType::Textures, name);
  }

  static bool exists(ResourceType type, const std::string &name) {
    std::string path = std::string(RESOURCES_PATH) +
                       resource_type_to_string(type) + "/" + name;

    return fs::exists(path);
  }

private:
  static std::string get_resource_path(ResourceType type,
                                       const std::string &name) {
    std::string path = std::string(RESOURCES_PATH) +
                       resource_type_to_string(type) + "/" + name;

    if (!fs::exists(path)) {
      throw std::runtime_error("Resource does not exist: " + path);
    }
    return path;
  }

  static std::string resource_type_to_string(ResourceType type) {
    switch (type) {
    case ResourceType::Shaders:
      return "shaders";
    case ResourceType::Images:
      return "images";
    case ResourceType::Textures:
      return "textures";
    default:
      throw std::runtime_error("Unknown resource type: " +
                               std::to_string((int)type));
    }
  }
};
