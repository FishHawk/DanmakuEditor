#pragma once

#include <string>

#include <entt/entt.hpp>

#include "../util/base.hpp"
#include "Event.hpp"
#include "Listener.hpp"

using WindowHandle = GLFWwindow *;

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

  void set_title(const std::string &title);

  Vec2i position() const;
  void set_position(const Vec2i &position);

  Vec2i size() const;
  void set_size(const Vec2i &size);

  void iconify();
  void restore();
  void maximize();

  void show();
  void hide();
  void focus();

  void poll_events();
  void wait_events();
  void wait_events(double timeout);

  bool is_key_pressed(Key key);

  bool is_mouse_button_pressed(MouseButton button);

  Vec2f get_mouse_position();
  void set_mouse_position(const Vec2f &position);

  template <typename Event, typename Listener>
  requires entt::Listenable<Event, Listener> void
  add_listener(Listener *listener) {
    entt::add_listener<Event>(_dispatcher, listener);
  }

  template <typename Event, typename Listener>
  requires entt::Listenable<Event, Listener> void
  remove_listener(Listener *listener) {
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

  WindowHandle _handle;
};