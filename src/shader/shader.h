#pragma once

#include <string>
#include <vector>
#include <renderer/renderer.h>

enum class ShaderType {
  Vertex = GL_VERTEX_SHADER,
  Fragment = GL_FRAGMENT_SHADER,
};

class Shader {
public:
  Shader();
  ~Shader();

  Shader &add_shader(const std::string &file_path, ShaderType type);
  void compile_and_link();
  void use() const;
  void set_uniform(const std::string &name, float value);
  void set_uniform(const std::string &name, float v1, float v2);
  void set_uniform(const std::string &name, float v1, float v2, float v3);
  void set_uniform(const std::string &name, float v1, float v2, float v3,
                   float v4);

private:
  unsigned int _id;
  std::vector<unsigned int> shaders;

  void check_compile_errors(unsigned int shader, std::string type);
  std::string read_shader_from_file(const std::string &path);
};
