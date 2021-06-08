#pragma once

#include <string>

#include <entt/entt.hpp>

#include "../util/base.hpp"
#include "Event.hpp"
#include "Listener.hpp"

class Window {
public:
  Window(int width, int height, const std::string &title);

  /* context */
  void make_context_current();

  /* window */
  bool should_close();
  void set_should_close();

  void set_title(const std::string &title);

  Vec2i position() const;
  void set_position(const Vec2i &position);

  Vec2i size() const;
  void set_size(const Vec2i &size);

  Vec2i framebuffer_size() const;

  Vec2f content_scale() const;

  void iconify();
  void restore();
  void maximize();

  void show();
  void hide();
  void focus();
  void request_attention();

  void poll_events();
  void wait_events();
  void wait_events(double timeout);

  void display();

  /* input */
  static std::string get_clipboard_string();
  static void set_clipboard_string(const std::string& text);

  bool is_key_pressed(Key key);

  bool is_mouse_button_pressed(MouseButton button);

  Vec2f get_cursor_position();
  void set_cursor_position(const Vec2f &position);

  template <typename Event, typename Listener>
  requires entt::Listenable<Event, Listener>
  void add_listener(Listener *listener) {
    entt::add_listener<Event>(*_dispatcher, listener);
  }

  template <typename Event, typename Listener>
  requires entt::Listenable<Event, Listener>
  void remove_listener(Listener *listener) {
    entt::remove_listener<Event>(*_dispatcher, listener);
  }

  template <typename Listener>
  void add_listener(Listener *listener) {
    add_listener<WindowPositionEvent>(listener);
    add_listener<WindowSizeEvent>(listener);
    add_listener<WindowCloseEvent>(listener);
    add_listener<WindowRefreshEvent>(listener);
    add_listener<WindowFocusEvent>(listener);
    add_listener<WindowIconifyEvent>(listener);
    add_listener<WindowMaximizeEvent>(listener);
    add_listener<WindowFramebufferSizeEvent>(listener);
    add_listener<WindowContentScaleEvent>(listener);

    add_listener<KeyEvent>(listener);
    add_listener<TextEvent>(listener);
    add_listener<MouseButtonEvent>(listener);
    add_listener<CursorMoveEvent>(listener);
    add_listener<CursorEnterEvent>(listener);
    add_listener<ScrollEvent>(listener);

    add_listener<DropEvent>(listener);
  }

  template <typename Listener>
  void remove_listener(Listener *listener) {
    remove_listener<WindowPositionEvent>(listener);
    remove_listener<WindowSizeEvent>(listener);
    remove_listener<WindowCloseEvent>(listener);
    remove_listener<WindowRefreshEvent>(listener);
    remove_listener<WindowFocusEvent>(listener);
    remove_listener<WindowIconifyEvent>(listener);
    remove_listener<WindowMaximizeEvent>(listener);
    remove_listener<WindowFramebufferSizeEvent>(listener);
    remove_listener<WindowContentScaleEvent>(listener);

    remove_listener<KeyEvent>(listener);
    remove_listener<TextEvent>(listener);
    remove_listener<MouseButtonEvent>(listener);
    remove_listener<CursorMoveEvent>(listener);
    remove_listener<CursorEnterEvent>(listener);
    remove_listener<ScrollEvent>(listener);

    remove_listener<DropEvent>(listener);
  }

private:
  template <typename Event, typename Listener>
  void add_listener(Listener *listener) {}

  template <typename Event, typename Listener>
  void remove_listener(Listener *listener) {}

  entt::dispatcher *_dispatcher;
  std::shared_ptr<GLFWwindow> _handle;
};