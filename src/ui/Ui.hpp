#pragma once

#include <array>
#include <deque>
#include <map>
#include <string>

#include <entt/entt.hpp>

class Ui {
public:
  static Ui &get_instance() {
    static Ui ui;
    return ui;
  }

  void update(entt::registry &registry);

  void append_system_time(const std::string &name, double value);

  void toggle_console_window() {
    _is_console_window_visible = !_is_console_window_visible;
  }

private:
  Ui();
  ~Ui();
  Ui(const Ui &other) = delete;
  Ui &operator=(const Ui &) = delete;

  static constexpr size_t record_time = 5;

  const std::map<std::string, size_t> _system_names{
      {"All", 0}, {"Move", 1}, {"Launch", 2}, {"Render", 3}, {"Ui", 4}};
  std::array<std::deque<double>, record_time> _system_times;

  const std::map<std::string, size_t> _component_names{
      {"Entity", 0},
      {"Renderable", 1},
      {"Moveable", 2},
      {"Launchable", 3},
      {"Livetime", 4}};
  std::array<std::deque<unsigned int>, record_time> _component_numbers;

  void append_component_number(const std::string &name, unsigned int value);

  bool _is_console_window_visible = false;
  void update_console_window(entt::registry &registry);
};