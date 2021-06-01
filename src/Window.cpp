#include "Window.hpp"

#include <utility>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "Game.hpp"

static size_t window_count = 0;

void on_glfw_error(int error_code, const char *description) {
  spdlog::error("GLFW #{}: {}", error_code, description);
}

void Window::poll_events() { glfwPollEvents(); };

void Window::wait_events() { glfwWaitEvents(); };

Window::Window(int width, int height, std::string title) {
  if (window_count == 0) {
    glfwSetErrorCallback(on_glfw_error);
    if (!glfwInit()) {
      spdlog::error("Failed to init GLFW");
      exit(EXIT_FAILURE);
    }
  }

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  _handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!_handle) {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(_handle);

  // initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::error("Failed to initialize GLAD");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(_handle, Game::framebuffer_size_callback);
  glfwSetCursorPosCallback(_handle, Game::mouse_callback);
  glfwSetScrollCallback(_handle, Game::scroll_callback);
  glfwSetKeyCallback(_handle, Game::key_callback);

  ++window_count;
}

Window::~Window() {
  glfwDestroyWindow(_handle);

  if (_handle) {
    --window_count;
    if (window_count == 0) {
      glfwTerminate();
    }
  }
}

Window::Window(Window &&other)
    : _handle(std::exchange(other._handle, nullptr)){};

void Window::make_context_current() { glfwMakeContextCurrent(_handle); }

bool Window::should_close() const { return glfwWindowShouldClose(_handle); }

void Window::close() { glfwSetWindowShouldClose(_handle, true); }

void Window::display() { glfwSwapBuffers(_handle); }

bool Window::is_key_pressed(Key key) {
  return glfwGetKey(_handle, static_cast<int>(key));
}