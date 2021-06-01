#define GLFW_INCLUDE_NONE
#include "Window.hpp"

#include <map>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

std::map<GLFWwindow *, entt::dispatcher &> dispatchers;

void on_glfw_error(int error_code, const char *description) {
  spdlog::error("GLFW #{}: {}", error_code, description);
}

void on_resize_event(GLFWwindow *window, int width, int height) {
  auto &dispatcher = dispatchers.at(window);
  dispatcher.enqueue<ResizeEvent>(width, height);
}

void on_key_event(
    GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto &dispatcher = dispatchers.at(window);
  dispatcher.enqueue<KeyEvent>(Key(key), KeyState(action), mods);
}

void on_mouse_move_event(GLFWwindow *window, double xpos, double ypos) {
  auto &dispatcher = dispatchers.at(window);
  dispatcher.enqueue<MouseMoveEvent>(xpos, ypos);
}

void on_mouse_button_event(
    GLFWwindow *window, int button, int action, int mods) {
  auto &dispatcher = dispatchers.at(window);
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  dispatcher.enqueue<MouseButtonEvent>(
      MouseButton(button), MouseButtonState(action), mods, xpos, ypos);
}

void on_scroll_event(GLFWwindow *window, double xoffset, double yoffset) {
  auto &dispatcher = dispatchers.at(window);
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  if (xoffset != 0) {
    dispatcher.enqueue<ScrollEvent>(ScrollEvent::Axis::X, xoffset, xpos, ypos);
  }
  if (yoffset != 0) {
    dispatcher.enqueue<ScrollEvent>(ScrollEvent::Axis::Y, yoffset, xpos, ypos);
  }
}

Window::Window(int width, int height, const std::string &title) {
  if (dispatchers.empty()) {
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

  dispatchers.emplace(_handle, dispatcher);
  dispatcher.enqueue<ResizeEvent>(width, height);

  glfwSetFramebufferSizeCallback(_handle, on_resize_event);
  glfwSetKeyCallback(_handle, on_key_event);
  glfwSetCursorPosCallback(_handle, on_mouse_move_event);
  glfwSetMouseButtonCallback(_handle, on_mouse_button_event);
  glfwSetScrollCallback(_handle, on_scroll_event);
}

Window::~Window() {
  glfwDestroyWindow(_handle);

  dispatchers.erase(_handle);
  if (dispatchers.empty()) {
    glfwTerminate();
  }
}

void Window::make_context_current() { glfwMakeContextCurrent(_handle); }

bool Window::should_close() const { return glfwWindowShouldClose(_handle); }

void Window::close() { glfwSetWindowShouldClose(_handle, true); }

void Window::display() { glfwSwapBuffers(_handle); }

bool Window::is_key_pressed(Key key) {
  return glfwGetKey(_handle, static_cast<int>(key));
}

bool Window::is_mouse_button_pressed(MouseButton button) {
  return glfwGetMouseButton(_handle, static_cast<int>(button));
}

Vec2f Window::get_mouse_position() {
  double xpos, ypos;
  glfwGetCursorPos(_handle, &xpos, &ypos);
  return {xpos, ypos};
}

void Window::set_mouse_position(const Vec2f &position) {
  glfwSetCursorPos(_handle, position.x, position.y);
}

void Window::poll_events() {
  glfwPollEvents();
  dispatcher.update();
}