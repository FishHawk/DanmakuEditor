#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <entt/entt.hpp>
#include <glad/glad.h>

#include "SpellManager.hpp"
#include "Window.hpp"
#include "graphic/Renderer.hpp"
#include "resources/ResourceManager.hpp"

class Game {
public:
  static Game &get_instance() {
    static Game game;
    return game;
  }

  void loop();

  static void
  framebuffer_size_callback(GLFWwindow *window, int width, int height);
  static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
  static void
  scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
  static void
  key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

  void run_spell(size_t index);
  const auto &spell_manager() { return _spell_manager; }

  auto &camera() { return _renderer.camera(); }
  auto &window() { return _window; }

private:
  Game();
  ~Game() = default;
  Game(const Game &other) = delete;
  Game &operator=(const Game &) = delete;

  void process_input();
  void update_with_timer(
      const std::string &name, std::function<void(void)> callback);
  void render_ui();

  const int _width = 600, _height = 800;
  Window _window{_width, _height, "Danmaku Editor"};

  GLboolean _keys[1024];
  entt::registry _registry;

  ResourceManager _resource_manager;
  SpellManager _spell_manager;

  Renderer _renderer;
};