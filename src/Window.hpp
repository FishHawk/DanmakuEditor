#pragma once

#define GLFW_INCLUDE_NONE

#include <string>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <implot.h>

#include "KeyEvent.hpp"

class Window {
public:
  Window(int width, int height, std::string title);

  ~Window();

  bool should_close() const;

  void close();

  void display();

  void poll_events();

  bool is_key_pressed(Key key);

private:
  GLFWwindow *_window;
  int _width, _height;
  std::string _title;
};