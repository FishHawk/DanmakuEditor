#pragma once

#include <string>

#include <entt/entt.hpp>

#include "../util/base.hpp"
#include "Event.hpp"

class Window {
public:
  Window(int width, int height, const std::string &title);
  ~Window();

  Window(const Window &other) = delete;
  Window &operator=(const Window &) = delete;

  Window(Window &&other) = delete;
  Window &operator=(Window &&) = delete;

  void make_context_current();

  bool should_close() const;

  void close();

  void display();

  bool is_key_pressed(Key key);

  bool is_mouse_button_pressed(MouseButton button);

  Vec2f get_mouse_position();
  void set_mouse_position(const Vec2f &position);

  void poll_events();

  entt::dispatcher dispatcher;

private:
  static void init();
  static void terminate();

  GLFWwindow *_handle;
};