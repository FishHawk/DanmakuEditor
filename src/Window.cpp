#include "Window.hpp"

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
}

Window::~Window() {
  glfwDestroyWindow(_window);
  glfwTerminate();
}

bool Window::should_close() const { return glfwWindowShouldClose(_window); }

void Window::close() { glfwSetWindowShouldClose(_window, true); }

void Window::display() { glfwSwapBuffers(_window); }

void Window::poll_events() { glfwPollEvents(); }

bool Window::is_key_pressed(Key key) {
  return glfwGetKey(_window, static_cast<int>(key));
}