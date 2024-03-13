#include "texture/texture_2d.h"
#include "utils/R.h"
#include <buffer/index_buffer.h>
#include <buffer/vertex_array.h>
#include <buffer/vertex_buffer.h>
#include <buffer/vertex_buffer_layout.h>
#include <config/constants.h>
#include <renderer/renderer.h>
#include <renderer/utils.h>
#include <shader/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

  const float vertices[] = {
      // positions       // texture coords
      100.0f, 100.0f, 0.0f, 1.0f, // top right
      100.0f, 0.0f,   0.0f, 0.0f, // bottom right
      0.0f,   0.0f,   1.0f, 0.0f, // bottom left
      0.0f,   100.0f, 1.0f, 1.0f  // top left
  };

  const unsigned int indices[] = {
      0, 1, 2, // 0
      2, 3, 0  // 1
  };

  VertexBuffer vertex_buffer(vertices, 4 * 4 * sizeof(float));
  VertexArray vertex_array;

  VertexBufferLayout layout;
  layout.push<float>(2);
  layout.push<float>(2);
  vertex_array.add_buffer(vertex_buffer, layout);

  IndexBuffer index_buffer(indices, 6);

  GL_CALL(glEnable(GL_BLEND));
  GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  glm::mat4 proj = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f,
                              (float)WINDOW_HEIGHT, -1.0f, 1.0f);
  glm::mat4 view =
      glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
  glm::mat4 model =
      glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));

  glm::mat4 mvp = proj * view * model;

  Shader shader;
  shader
      .add_shader(R::shaders("default/vertex.vert"), ShaderType::Vertex) //
      .add_shader(R::shaders("default/fragment.frag"), ShaderType::Fragment)
      .compile_and_link();

  shader.set_uniform("u_MVP", mvp);

  Texture2D texture(R::textures("arc.png"));
  texture.bind();
  shader.set_uniform("u_Texture", 0);

  Renderer renderer;

  vertex_array.unbind();
  vertex_buffer.unbind();
  index_buffer.unbind();
  shader.unbind();

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Rendering
    renderer.clear();
    renderer.draw(vertex_array, index_buffer, shader);

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
