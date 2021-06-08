#define GLFW_INCLUDE_NONE
#include "Window.hpp"

#include <map>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

std::map<GLFWwindow *, entt::dispatcher *> dispatchers;

void on_glfw_error(int error_code, const char *description) {
  spdlog::error("GLFW #{}: {}", error_code, description);
}

void on_resize_event(GLFWwindow *window, int width, int height) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<ResizeEvent>(width, height);
}

void on_key_event(
    GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<KeyEvent>(Key(key), KeyState(action), mods);
}

void on_mouse_move_event(GLFWwindow *window, double xpos, double ypos) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<MouseMoveEvent>(xpos, ypos);
}

void on_mouse_button_event(
    GLFWwindow *window, int button, int action, int mods) {
  auto dispatcher = dispatchers.at(window);
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  dispatcher->enqueue<MouseButtonEvent>(
      MouseButton(button), MouseButtonState(action), mods, xpos, ypos);
}

void on_scroll_event(GLFWwindow *window, double xoffset, double yoffset) {
  auto dispatcher = dispatchers.at(window);
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  if (xoffset != 0) {
    dispatcher->enqueue<ScrollEvent>(ScrollEvent::Axis::X, xoffset, xpos, ypos);
  }
  if (yoffset != 0) {
    dispatcher->enqueue<ScrollEvent>(ScrollEvent::Axis::Y, yoffset, xpos, ypos);
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

  auto p = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!p) {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  _handle = std::shared_ptr<GLFWwindow>{
      p,
      [](GLFWwindow *p) {
        glfwDestroyWindow(p);
        delete dispatchers.at(p);
        dispatchers.erase(p);
        if (dispatchers.empty()) {
          glfwTerminate();
        }
      }};

  // initialize GLAD
  make_context_current();
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::error("Failed to initialize GLAD");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  _dispatcher = new entt::dispatcher;
  _dispatcher->enqueue<ResizeEvent>(width, height);
  dispatchers.emplace(_handle.get(), _dispatcher);

  glfwSetFramebufferSizeCallback(_handle.get(), on_resize_event);
  glfwSetKeyCallback(_handle.get(), on_key_event);
  glfwSetCursorPosCallback(_handle.get(), on_mouse_move_event);
  glfwSetMouseButtonCallback(_handle.get(), on_mouse_button_event);
  glfwSetScrollCallback(_handle.get(), on_scroll_event);
}

void Window::make_context_current() {
  glfwMakeContextCurrent(_handle.get());
}

bool Window::should_close() {
  return glfwWindowShouldClose(_handle.get());
}

void Window::set_should_close() {
  glfwSetWindowShouldClose(_handle.get(), true);
}

void Window::display() { glfwSwapBuffers(_handle.get()); }

void Window::set_title(const std::string &title) {
  glfwSetWindowTitle(_handle.get(), title.c_str());
}

Vec2i Window::position() const {
  int x, y;
  glfwGetWindowPos(_handle.get(), &x, &y);
  return {x, y};
}

void Window::set_position(const Vec2i &position) {
  glfwSetWindowPos(_handle.get(), position.x, position.y);
}

Vec2i Window::size() const {
  int w, h;
  glfwGetWindowSize(_handle.get(), &w, &h);
  return {w, h};
}

void Window::set_size(const Vec2i &size) {
  glfwSetWindowSize(_handle.get(), size.x, size.y);
}

void Window::iconify() { glfwIconifyWindow(_handle.get()); }
void Window::restore() { glfwRestoreWindow(_handle.get()); }
void Window::maximize() { glfwMaximizeWindow(_handle.get()); }

void Window::show() { glfwShowWindow(_handle.get()); }
void Window::hide() { glfwHideWindow(_handle.get()); }
void Window::focus() { glfwFocusWindow(_handle.get()); }

void Window::poll_events() {
  glfwPollEvents();
  _dispatcher->update();
}

void Window::wait_events() {
  glfwWaitEvents();
  _dispatcher->update();
}

void Window::wait_events(double timeout) {
  glfwWaitEventsTimeout(timeout);
  _dispatcher->update();
}

bool Window::is_key_pressed(Key key) {
  return glfwGetKey(_handle.get(), static_cast<int>(key));
}

bool Window::is_mouse_button_pressed(MouseButton button) {
  return glfwGetMouseButton(_handle.get(), static_cast<int>(button));
}

Vec2f Window::get_mouse_position() {
  double xpos, ypos;
  glfwGetCursorPos(_handle.get(), &xpos, &ypos);
  return {xpos, ypos};
}

void Window::set_mouse_position(const Vec2f &position) {
  glfwSetCursorPos(_handle.get(), position.x, position.y);
}