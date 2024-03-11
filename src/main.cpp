#include "buffer/index_buffer.h"
#include "buffer/vertex_buffer.h"
#include "config/constants.h"
#include "renderer/renderer.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdio>
#include <glad/glad.h>
#include <iostream>

const char *vertex_shader_source = "#version 330 core\n"
                                   "layout (location = 0) in vec3 aPos;\n"
                                   " void main()\n"
                                   " {\n"
                                   " gl_Position = vec4(aPos.xyz, 1.0f);\n"
                                   " }\n";

const char *fragment_shader_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    " void main()\n"
    " {\n"
    " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    " }\n";

void c_speed(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

static float aspect_ratio =
    static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);

int main(int argc, char **argv) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW Window.\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD.\n");
    return -1;
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  glfwSetFramebufferSizeCallback(window, c_speed);

  // just drawing a square (two triangles) on the whole screen
  const float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
  };

  const unsigned int indices[] = {0, 1, 2, 1, 2, 3};

  unsigned int VAO;
  GL_CALL(glGenVertexArrays(1, &VAO));
  GL_CALL(glBindVertexArray(VAO));

  VertexBuffer vertex_buffer(vertices, sizeof(vertices));
  IndexBuffer index_buffer(indices, 6);

  GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                                (void *)0));
  GL_CALL(glEnableVertexAttribArray(0));

  vertex_buffer.unbind();
  index_buffer.unbind();
  GL_CALL(glBindVertexArray(0));

  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  GL_CALL(glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL));
  GL_CALL(glCompileShader(vertex_shader));

  int success;
  char info_log[512];
  GL_CALL(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
  if (!success) {
    GL_CALL(glGetShaderInfoLog(vertex_shader, 512, NULL, info_log));
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << info_log << std::endl;
  }

  unsigned int fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  GL_CALL(glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL));
  GL_CALL(glCompileShader(fragment_shader));

  GL_CALL(glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success));
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << info_log << std::endl;
  }

  unsigned int shader_program;
  shader_program = glCreateProgram();
  GL_CALL(glAttachShader(shader_program, vertex_shader));
  GL_CALL(glAttachShader(shader_program, fragment_shader));
  GL_CALL(glLinkProgram(shader_program));

  GL_CALL(glGetProgramiv(shader_program, GL_LINK_STATUS, &success));
  if (!success) {
    GL_CALL(glGetProgramInfoLog(shader_program, 512, NULL, info_log));
    std::cout << "ERROR::PROGRAM::SHADER_PROGRAM::LINKING_FAILED\n"
              << info_log << std::endl;
  }

  GL_CALL(glUseProgram(shader_program));
  GL_CALL(glDeleteShader(vertex_shader));
  GL_CALL(glDeleteShader(fragment_shader));

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Rendering
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    GL_CALL(glUseProgram(shader_program));
    GL_CALL(glBindVertexArray(VAO));
    index_buffer.bind();
    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    GL_CALL(glBindVertexArray(0));

    // Check and Events + Buf Swapping
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void c_speed(GLFWwindow *window, int width, int height) {
  aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
