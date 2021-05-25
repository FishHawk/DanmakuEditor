#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <entt/entt.hpp>
#include <glad/glad.h>

#include "TestCase.hpp"
#include "Window.hpp"
#include "graphic/Program.hpp"
#include "graphic/Texture.hpp"
#include "resources/ResourceManager.hpp"

class Game {
public:
  enum State { ACTIVE, PAUSE };

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

  void run_test_case(size_t index);

private:
  Game();
  ~Game() = default;
  Game(const Game &other) = delete;
  Game &operator=(const Game &) = delete;

  void process_input();
  void update_with_timer(
      const std::string &name, std::function<void(void)> callback);
  void render_ui();

  const int _width = 1110, _height = 800;
  Window _window{_width, _height, "Danmaku Editor"};

  State _state;
  GLboolean _keys[1024];
  entt::registry _registry;

  ResourceManager _resource_manager;
  entt::resource_cache<Program> _program_cache;
  entt::resource_cache<Texture> _texture_cache;

public:
  TestCase test_case;
};