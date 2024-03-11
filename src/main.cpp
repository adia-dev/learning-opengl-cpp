#include "buffer/index_buffer.h"
#include "buffer/vertex_array.h"
#include "buffer/vertex_buffer.h"
#include "buffer/vertex_buffer_layout.h"
#include "config/constants.h"
#include "renderer/renderer.h"
#include "shader/shader.h"

#include <cstddef>
#include <cstdio>
#include <glad/glad.h>

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

  VertexBuffer vertex_buffer(vertices, sizeof(vertices));
  IndexBuffer index_buffer(indices, 6);
  VertexArray vertex_array;

  VertexBufferLayout layout;
  layout.push<float>(2);
  vertex_array.add_buffer(vertex_buffer, layout);

  vertex_buffer.unbind();
  index_buffer.unbind();
  GL_CALL(glBindVertexArray(0));

  Shader shader;
  shader
      .add_shader("vertex.glsl", ShaderType::Vertex) //
      .add_shader("fragment.glsl", ShaderType::Fragment)
      .compile_and_link();

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Rendering
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    float timeValue = glfwGetTime();
    shader.use();
    shader.set_uniform("time", timeValue);

    shader.use();

    vertex_array.bind();
    index_buffer.bind();

    GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

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
