#pragma once

#define GLFW_INCLUDE_NONE

#include <string>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <implot.h>

class Window {
public:
  Window(int width, int height, std::string title);
  ~Window();

  bool is_open() const { return !glfwWindowShouldClose(_window); }

  void close() const { glfwSetWindowShouldClose(_window, true); };

  void display() { glfwSwapBuffers(_window); }
  void poll_events() { glfwPollEvents(); }

private:
  GLFWwindow *_window;
  int _width, _height;
  std::string _title;

  ImGuiContext *_ctx_imgui;
  ImPlotContext *_ctx_implot;
};