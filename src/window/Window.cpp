#define GLFW_INCLUDE_NONE
#include "Window.hpp"

#include <map>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

std::map<GLFWwindow *, entt::dispatcher *> dispatchers;

void on_glfw_error(int error_code, const char *description) {
  spdlog::error("GLFW #{}: {}", error_code, description);
}

void on_position_event(GLFWwindow *window, int x, int y) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<WindowPositionEvent>(x, y);
}

void on_size_event(GLFWwindow *window, int width, int height) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<WindowSizeEvent>(width, height);
}

void on_close_event(GLFWwindow *window) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<WindowCloseEvent>();
}

void on_refresh_event(GLFWwindow *window) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<WindowRefreshEvent>();
}

void on_focus_event(GLFWwindow *window, int focus) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<WindowFocusEvent>(focus);
}

void on_iconify_event(GLFWwindow *window, int iconified) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<WindowIconifyEvent>(iconified);
}

void on_maximize_event(GLFWwindow *window, int maximized) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<WindowMaximizeEvent>(maximized);
}

void on_framebuffer_size_event(GLFWwindow *window, int width, int height) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<WindowFramebufferSizeEvent>(width, height);
}

void on_content_scale_event(GLFWwindow *window, float xscale, float yscale) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<WindowContentScaleEvent>(xscale, yscale);
}

void on_key_event(GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<KeyEvent>(Key(key), KeyState(action), mods);
}

void on_text_event(GLFWwindow *window, unsigned int codepoint) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<TextEvent>(codepoint);
}

void on_mouse_button_event(GLFWwindow *window, int button, int action, int mods) {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<MouseButtonEvent>(
      MouseButton(button),
      MouseButtonState(action),
      mods, xpos, ypos);
}

void on_cursor_move_event(GLFWwindow *window, double xpos, double ypos) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<CursorMoveEvent>(xpos, ypos);
}

void on_cursor_enter_event(GLFWwindow *window, int entered) {
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<CursorEnterEvent>(entered);
}

void on_scroll_event(GLFWwindow *window, double xoffset, double yoffset) {
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  auto dispatcher = dispatchers.at(window);
  if (xoffset != 0) {
    dispatcher->enqueue<ScrollEvent>(ScrollEvent::Axis::X, xoffset, xpos, ypos);
  }
  if (yoffset != 0) {
    dispatcher->enqueue<ScrollEvent>(ScrollEvent::Axis::Y, yoffset, xpos, ypos);
  }
}

void on_drop_event(GLFWwindow *window, int count, const char **paths) {
  std::vector<std::string> vec(paths, paths + count);
  auto dispatcher = dispatchers.at(window);
  dispatcher->enqueue<DropEvent>(vec);
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
  _dispatcher->enqueue<WindowSizeEvent>(width, height);
  _dispatcher->enqueue<WindowFramebufferSizeEvent>(width, height);
  dispatchers.emplace(_handle.get(), _dispatcher);

  glfwSetWindowPosCallback(p, on_position_event);
  glfwSetWindowSizeCallback(p, on_size_event);
  glfwSetWindowCloseCallback(p, on_close_event);
  glfwSetWindowRefreshCallback(p, on_refresh_event);
  glfwSetWindowFocusCallback(p, on_focus_event);
  glfwSetWindowIconifyCallback(p, on_iconify_event);
  glfwSetWindowMaximizeCallback(p, on_maximize_event);
  glfwSetFramebufferSizeCallback(p, on_framebuffer_size_event);
  glfwSetWindowContentScaleCallback(p, on_content_scale_event);

  glfwSetKeyCallback(p, on_key_event);
  glfwSetCharCallback(p, on_text_event);
  glfwSetMouseButtonCallback(p, on_mouse_button_event);
  glfwSetCursorPosCallback(p, on_cursor_move_event);
  glfwSetCursorEnterCallback(p, on_cursor_enter_event);
  glfwSetScrollCallback(p, on_scroll_event);
  glfwSetDropCallback(p, on_drop_event);
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

Vec2i Window::framebuffer_size() const {
  int w, h;
  glfwGetFramebufferSize(_handle.get(), &w, &h);
  return {w, h};
}

Vec2f Window::content_scale() const {
  float xscale, yscale;
  glfwGetWindowContentScale(_handle.get(), &xscale, &yscale);
  return {xscale, yscale};
}

void Window::iconify() { glfwIconifyWindow(_handle.get()); }
void Window::restore() { glfwRestoreWindow(_handle.get()); }
void Window::maximize() { glfwMaximizeWindow(_handle.get()); }

void Window::show() { glfwShowWindow(_handle.get()); }
void Window::hide() { glfwHideWindow(_handle.get()); }
void Window::focus() { glfwFocusWindow(_handle.get()); }
void Window::request_attention() { glfwRequestWindowAttention(_handle.get()); }

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

void Window::display() {
  glfwSwapBuffers(_handle.get());
}

std::string Window::get_clipboard_string() {
  return glfwGetClipboardString(NULL);
}

void Window::set_clipboard_string(const std::string &text) {
  glfwSetClipboardString(NULL, text.c_str());
}

bool Window::is_key_pressed(Key key) {
  return glfwGetKey(_handle.get(), static_cast<int>(key));
}

bool Window::is_mouse_button_pressed(MouseButton button) {
  return glfwGetMouseButton(_handle.get(), static_cast<int>(button));
}

Vec2f Window::get_cursor_position() {
  double xpos, ypos;
  glfwGetCursorPos(_handle.get(), &xpos, &ypos);
  return {xpos, ypos};
}

void Window::set_cursor_position(const Vec2f &position) {
  glfwSetCursorPos(_handle.get(), position.x, position.y);
}