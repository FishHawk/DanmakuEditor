#pragma once

#define GLFW_INCLUDE_NONE

#include <string>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Window {
public:
  Window(int width, int height, std::string title);
  ~Window() { glfwTerminate(); };

  bool should_close() { return glfwWindowShouldClose(_window); }
  void swap_buffers() { glfwSwapBuffers(_window); }
  void poll_events() { glfwPollEvents(); }

private:
  GLFWwindow *_window;
  int _width, _height;
  std::string _title;
};