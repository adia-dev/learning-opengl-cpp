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
#include <texture/texture_2d.h>
#include <utils/R.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void c_speed(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

static float aspect_ratio =
    static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);

static const char *glsl_version = "#version 330";

int main(int argc, char **argv) {
  unsigned int window_width = WINDOW_WIDTH;
  unsigned int window_height = WINDOW_HEIGHT;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--width") == 0 && i + 1 < argc) {
      window_width = atoi(argv[i + 1]);
    } else if (strcmp(argv[i], "--height") == 0 && i + 1 < argc) {
      window_height = atoi(argv[i + 1]);
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

  glfwSetFramebufferSizeCallback(window, c_speed);

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

  glm::vec3 translate(200.0f, 200.0f, 0.0f);

  Shader shader;
  shader
      .add_shader(R::shaders("default/vertex.vert"), ShaderType::Vertex) //
      .add_shader(R::shaders("default/fragment.frag"), ShaderType::Fragment)
      .compile_and_link();

  Texture2D texture(R::textures("arc.png"));
  texture.bind();
  shader.set_uniform("u_Texture", 0);

  Renderer renderer;

  vertex_array.unbind();
  vertex_buffer.unbind();
  index_buffer.unbind();
  shader.unbind();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    processInput(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Rendering
    renderer.clear();
    renderer.draw(vertex_array, index_buffer, shader);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), translate);
    glm::mat4 mvp = proj * view * model;

    shader.bind();
    shader.set_uniform("u_MVP", mvp);

    {
      static float f = 0.0f;
      static int counter = 0;
      static bool show_window = 0;

      ImGui::Begin("Transform", &show_window);

      ImGui::SliderFloat3("Translation", &translate[0], 0.0f,
                          (float)window_width);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);

      ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
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
