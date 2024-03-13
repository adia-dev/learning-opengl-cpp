#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>

enum class ShaderType {
  Vertex = 35633,
  Fragment = 35632,
};

class Shader {
public:
  Shader();
  ~Shader();

  Shader &add_shader(const std::string &file_path, ShaderType type);
  void compile_and_link();
  void bind() const;
  void unbind() const;
  void set_uniform(const std::string &name, int value);
  void set_uniform(const std::string &name, float value);
  void set_uniform(const std::string &name, float v1, float v2);
  void set_uniform(const std::string &name, float v1, float v2, float v3);
  void set_uniform(const std::string &name, float v1, float v2, float v3,
                   float v4);

  void set_uniform(const std::string &name, const glm::mat4 &mat);

private:
  unsigned int _id;
  std::vector<unsigned int> _shaders;
  std::unordered_map<std::string, int> _uniforms_location_cache;

  void check_compile_errors(unsigned int shader, std::string type);
  std::string read_shader_from_file(const std::string &path);
  int get_uniform_location(const std::string &name);
};
