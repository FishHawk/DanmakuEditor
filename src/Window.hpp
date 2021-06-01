#pragma once

#define GLFW_INCLUDE_NONE

#include <string>

#include "KeyEvent.hpp"

class Window {
public:
  static void poll_events();
  static void wait_events();

  Window(int width, int height, std::string title);
  ~Window();

  Window(const Window &other) = delete;
  Window &operator=(const Window &) = delete;

  Window(Window &&other);
  Window &operator=(Window &&) = delete;

  operator GLFWwindow *() { return _handle; }

  void make_context_current();

  bool should_close() const;

  void close();

  void display();

  bool is_key_pressed(Key key);

private:
  static void init();
  static void terminate();

  GLFWwindow *_handle;
};