#include "shader.h"

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <renderer/renderer.h>
#include <sstream>
#include <string>
#include <vector>

Shader::Shader() : _id(glCreateProgram()) {}

Shader::~Shader() {
  for (auto &shader : shaders) {
    GL_CALL(glDetachShader(_id, shader));
    GL_CALL(glDeleteShader(shader));
  }
  GL_CALL(glDeleteProgram(_id));
}

Shader &Shader::add_shader(const std::string &file_path, unsigned int type) {
  unsigned int shader = glCreateShader(type);
  std::string src = read_shader_from_file(file_path);
  const char *c_src = src.c_str();
  GL_CALL(glShaderSource(shader, 1, &c_src, nullptr));
  GL_CALL(glCompileShader(shader));

  GLint success;
  GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
  if (!success) {
    check_compile_errors(shader, "SHADER");
  }

  GL_CALL(glAttachShader(_id, shader));
  shaders.push_back(shader);
  return *this;
}

void Shader::compile_and_link() {
  GL_CALL(glLinkProgram(_id));
  check_compile_errors(_id, "PROGRAM");

  // After linking, shaders can be deleted
  for (auto &shader : shaders) {
    GL_CALL(glDetachShader(_id, shader));
    GL_CALL(glDeleteShader(shader));
  }
  shaders.clear();
}

void Shader::use() const { glUseProgram(_id); }

void Shader::set_uniform(const std::string &name, float value) {
  glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::set_uniform(const std::string &name, float v1, float v2) {
  glUniform2f(glGetUniformLocation(_id, name.c_str()), v1, v2);
}

void Shader::set_uniform(const std::string &name, float v1, float v2,
                         float v3) {
  glUniform3f(glGetUniformLocation(_id, name.c_str()), v1, v2, v3);
}

void Shader::set_uniform(const std::string &name, float v1, float v2, float v3,
                         float v4) {
  glUniform4f(glGetUniformLocation(_id, name.c_str()), v1, v2, v3, v4);
}

void Shader::check_compile_errors(unsigned int object, std::string type) {
  GLint success;
  GLchar info_log[1024];
  if (type != "PROGRAM") {
    GL_CALL(glGetShaderiv(object, GL_COMPILE_STATUS, &success));
    if (!success) {
      GL_CALL(glGetShaderInfoLog(object, 1024, nullptr, info_log));
      std::cout
          << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
          << info_log
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  } else {
    GL_CALL(glGetProgramiv(object, GL_LINK_STATUS, &success));
    if (!success) {
      GL_CALL(glGetProgramInfoLog(object, 1024, nullptr, info_log));
      std::cout
          << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
          << info_log
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  }
}

std::string Shader::read_shader_from_file(const std::string &path) {
  std::ifstream shader_file;
  std::stringstream shader_stream;
  shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    shader_file.open(path);
    shader_stream << shader_file.rdbuf();
    shader_file.close();

    return shader_stream.str();
  } catch (std::ifstream::failure &e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << path
              << std::endl;
    return "";
  }
}
