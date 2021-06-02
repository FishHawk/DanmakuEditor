#pragma once

#include <string>

#include <entt/entt.hpp>

#include "../util/base.hpp"
#include "Event.hpp"
#include "Listener.hpp"

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

  template <typename Event, typename Listener>
  requires entt::Listenable<Event, Listener>
  void add_listener(Listener *listener) {
    entt::add_listener<Event>(_dispatcher, listener);
  }

  template <typename Event, typename Listener>
  requires entt::Listenable<Event, Listener>
  void remove_listener(Listener *listener) {
    entt::remove_listener<Event>(_dispatcher, listener);
  }

  template <typename Listener>
  void add_listener(Listener *listener) {
    add_listener<ResizeEvent>(listener);
    add_listener<KeyEvent>(listener);
    add_listener<ScrollEvent>(listener);
    add_listener<MouseButtonEvent>(listener);
  }

  template <typename Listener>
  void remove_listener(Listener *listener) {
    remove_listener<ResizeEvent>(listener);
    remove_listener<KeyEvent>(listener);
    remove_listener<ScrollEvent>(listener);
    remove_listener<MouseButtonEvent>(listener);
  }

private:
  template <typename Event, typename Listener>
  void add_listener(Listener *listener) {}

  template <typename Event, typename Listener>
  void remove_listener(Listener *listener) {}

  entt::dispatcher _dispatcher;

  GLFWwindow *_handle;
};