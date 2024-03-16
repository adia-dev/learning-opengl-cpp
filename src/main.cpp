#include "tests/buffer_test.h"
#include "tests/test_manager.h"
#include "tests/texture_2d_test.h"
#include <cstddef>
#include <cstdio>

#include <buffer/index_buffer.h>
#include <buffer/vertex_array.h>
#include <buffer/vertex_buffer.h>
#include <buffer/vertex_buffer_layout.h>
#include <config/constants.h>
#include <renderer/renderer.h>
#include <renderer/utils.h>
#include <shader/shader.h>
#include <tests/clear_color_test.h>
#include <tests/test.h>
#include <texture/texture_2d.h>
#include <utils/R.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

static float aspect_ratio =
    static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);

static const char *glsl_version = "#version 330";

int main(int argc, char **argv) {
  unsigned int window_width = WINDOW_WIDTH;
  unsigned int window_height = WINDOW_HEIGHT;
  const char *arg_test = nullptr;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--width") == 0 && i + 1 < argc) {
      window_width = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "--height") == 0 && i + 1 < argc) {
      window_height = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "--test") == 0 && i + 1 < argc) {
      arg_test = argv[i + 1];
    }
  }

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(window_width, window_height, "OpenGL", NULL, NULL);
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

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
  ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
  ImGui_ImplOpenGL3_Init(glsl_version);

  GL_CALL(glEnable(GL_BLEND));
  GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  Renderer renderer;

  test::TestManager test_manager;
  test_manager.register_test<test::ClearColorTest>("Clear Color");
  test_manager.register_test<test::BufferTest>("Buffers");
  test_manager.register_test<test::Texture2DTest>("Texture2D");

  if (arg_test != nullptr) {
    test_manager.select_test(arg_test);
  }

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    renderer.clear();

    test_manager.render();
    test_manager.update(0.0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    test_manager.imgui_render();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
