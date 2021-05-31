#include "Game.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "components/Launchable.hpp"
#include "components/Livetime.hpp"
#include "components/Moveable.hpp"
#include "generators/DirectionGenerator.hpp"
#include "generators/ModifierGenerator.hpp"
#include "generators/OriginGenerator.hpp"
#include "resources/ProgramLoader.hpp"
#include "resources/TextureLoader.hpp"
#include "systems/LaunchSystem.hpp"
#include "systems/LivetimeSystem.hpp"
#include "systems/MoveSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "ui/Ui.hpp"
#include "util/Timer.hpp"
#include "util/debug.hpp"

using namespace entt::literals;

void Game::framebuffer_size_callback(
    GLFWwindow *window, int width, int height) {
  auto &camera = get_instance().camera();
  camera.set_screen_size(width, height);
  camera.set_size(0.5f * width, 0.5f * height);
}

void Game::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  static float xlast = 0;
  static float ylast = 0;
  static bool is_first = true;

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
    is_first = true;
    return;
  }

  if (is_first) {
    xlast = xpos;
    ylast = ypos;
    is_first = false;
  }

  float xoffset = xpos - xlast;
  float yoffset = ylast - ypos;

  xlast = xpos;
  ylast = ypos;
}

void Game::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  auto &camera = get_instance().camera();
  camera.zoom(1 + yoffset * 0.12);
}

void Game::key_callback(
    GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto &game = get_instance();

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    game.window().close();
  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS)
      game._keys[key] = GL_TRUE;
    else if (action == GLFW_RELEASE)
      game._keys[key] = GL_FALSE;
  }
}

Game::Game()
    : _resource_manager("/home/wh/Projects/DanmakuEditor/assets/"),
      _spell_manager(_resource_manager),
      _renderer(
          _resource_manager.program_cache(),
          _resource_manager.texture_cache(),
          _resource_manager.sprite_frame_cache()) {
  _renderer.camera().set_screen_size(_width, _height);
  _renderer.camera().set_size(0.5f * _width, 0.5f * _height);

  run_spell(0);
}

void Game::loop() {
  static LivetimeSystem livetime_system{_registry};
  static MoveSystem move_system{_registry};
  static LaunchSystem launch_system{_registry};
  static RenderSystem render_system{_registry, _renderer};

  util::Timer frame_timer, system_timer;

  while (_window.is_open()) {
    Ui::get_instance().append_system_time("All", frame_timer.elapsed());
    frame_timer.restart();

    _window.poll_events();
    process_input();

    livetime_system.update();

    update_with_timer("Launch", [this]() { launch_system.update(); });
    update_with_timer("Move", [this]() { move_system.update(); });
    update_with_timer("Render", [this]() { render_system.update(); });
    update_with_timer("Ui", [this]() { Ui::get_instance().update(_registry); });

    _window.display();
  }
}

void Game::process_input() {
  float speed = 5, rotate_speed = glm::radians(360.f) / 5 / 60;
  if (_keys[GLFW_KEY_W])
    _renderer.camera().move(0, -speed);
  if (_keys[GLFW_KEY_S])
    _renderer.camera().move(0, speed);
  if (_keys[GLFW_KEY_A])
    _renderer.camera().move(-speed, 0);
  if (_keys[GLFW_KEY_D])
    _renderer.camera().move(speed, 0);
  if (_keys[GLFW_KEY_Q])
    _renderer.camera().rotate(rotate_speed);
  if (_keys[GLFW_KEY_E])
    _renderer.camera().rotate(-rotate_speed);
}

void Game::update_with_timer(
    const std::string &name, std::function<void(void)> callback) {
  static util::Timer timer;
  timer.restart();
  callback();
  Ui::get_instance().append_system_time(name, timer.elapsed());
}

void Game::run_spell(size_t index) {
  _registry.clear();

  const auto spawn = _registry.create();
  _registry.emplace<Livetime>(spawn, 2);
  _registry.emplace<Launchable>(spawn, _spell_manager.spells.at(index));
  _registry.emplace<Moveable>(spawn, Position(0, 0));
}
