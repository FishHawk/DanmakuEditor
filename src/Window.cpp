#define GLFW_INCLUDE_NONE
#include "Window.hpp"

#include <map>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

std::map<GLFWwindow *, entt::dispatcher> input_dispatches;

void on_resize_event(GLFWwindow *window, int width, int height) {
  auto &dispatcher = input_dispatches.at(window);
  dispatcher.trigger<ResizeEvent>(width, height);
}

void on_key_event(
    GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto &dispatcher = input_dispatches.at(window);
  dispatcher.trigger<KeyEvent>(Key(key), KeyState(action), mods);
}

void on_mouse_move_event(GLFWwindow *window, double xpos, double ypos) {
  auto &dispatcher = input_dispatches.at(window);
  dispatcher.trigger<MouseMoveEvent>(xpos, ypos);
}

void on_mouse_button_event(
    GLFWwindow *window, int button, int action, int mods) {
  auto &dispatcher = input_dispatches.at(window);
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  dispatcher.trigger<MouseButtonEvent>(
      MouseButton(button), MouseButtonState(action), mods, xpos, ypos);
}

void on_scroll_event(GLFWwindow *window, double xoffset, double yoffset) {
  auto &dispatcher = input_dispatches.at(window);
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  if (xoffset != 0) {
    dispatcher.trigger<ScrollEvent>(ScrollEvent::Axis::X, xoffset, xpos, ypos);
  }
  if (yoffset != 0) {
    dispatcher.trigger<ScrollEvent>(ScrollEvent::Axis::Y, yoffset, xpos, ypos);
  }
}

void on_glfw_error(int error_code, const char *description) {
  spdlog::error("GLFW #{}: {}", error_code, description);
}

void Window::poll_events() { glfwPollEvents(); };

void Window::wait_events() { glfwWaitEvents(); };

Window::Window(int width, int height, std::string title) {
  if (input_dispatches.empty()) {
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

  input_dispatches.try_emplace(_handle);

  glfwSetFramebufferSizeCallback(_handle, on_resize_event);
  glfwSetKeyCallback(_handle, on_key_event);
  glfwSetCursorPosCallback(_handle, on_mouse_move_event);
  glfwSetMouseButtonCallback(_handle, on_mouse_button_event);
  glfwSetScrollCallback(_handle, on_scroll_event);
}

Window::~Window() {
  glfwDestroyWindow(_handle);

  if (_handle) {
    input_dispatches.erase(_handle);
    if (input_dispatches.empty()) {
      glfwTerminate();
    }
  }
}

void Window::make_context_current() { glfwMakeContextCurrent(_handle); }

bool Window::should_close() const { return glfwWindowShouldClose(_handle); }

void Window::close() { glfwSetWindowShouldClose(_handle, true); }

void Window::display() { glfwSwapBuffers(_handle); }

bool Window::is_key_pressed(Key key) {
  return glfwGetKey(_handle, static_cast<int>(key));
}

entt::dispatcher &Window::input_dispatcher() {
  return input_dispatches.at(_handle);
}