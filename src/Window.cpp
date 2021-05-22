#include "Window.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <spdlog/spdlog.h>

#include "Game.hpp"

Window::Window(int width, int height, std::string title)
    : _width(width), _height(height), _title(title) {
  glfwInit();
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
  if (!_window) {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
  }
  glfwMakeContextCurrent(_window);

  // initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::error("Failed to initialize GLAD");
    glfwTerminate();
  }

  glfwSetFramebufferSizeCallback(_window, Game::framebuffer_size_callback);
  glfwSetCursorPosCallback(_window, Game::mouse_callback);
  glfwSetScrollCallback(_window, Game::scroll_callback);
  glfwSetKeyCallback(_window, Game::key_callback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();

  // setup imgui io
  auto &io = ImGui::GetIO();
  io.IniFilename = nullptr;

  // setup imgui style
  ImGui::StyleColorsDark();

  // setup platform/renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(_window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
}
