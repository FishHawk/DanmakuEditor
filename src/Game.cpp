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
  glViewport(0, 0, width, height);
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
}

void Game::key_callback(
    GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto &game = get_instance();

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS)
      game._keys[key] = GL_TRUE;
    else if (action == GLFW_RELEASE)
      game._keys[key] = GL_FALSE;
  }
}

Game::Game()
    : _state(State::ACTIVE), _keys(),
      _resource_manager("/home/wh/Projects/DanmakuEditor/assets/"),
      _spell_manager(_resource_manager) {
  auto program = _resource_manager.program_cache().handle("base"_hs);
  glm::mat4 projection = glm::ortho(
      0.0f,
      static_cast<float>(_width),
      static_cast<float>(_height),
      0.0f,
      -1.0f,
      1.0f);
  program->use();
  program->set_int("image", 0);
  program->set_mat4("projection", projection);

  run_spell(0);
}

void Game::loop() {
  static LivetimeSystem livetime_system{_registry};
  static MoveSystem move_system{_registry};
  static LaunchSystem launch_system{_registry};
  static RenderSystem render_system{
      _registry,
      _resource_manager.program_cache(),
      _resource_manager.texture_cache()};

  util::Timer frame_timer, system_timer;

  while (!_window.should_close()) {
    Ui::get_instance().append_system_time("All", frame_timer.elapsed());
    frame_timer.restart();

    _window.poll_events();
    process_input();

    livetime_system.update();

    update_with_timer("Launch", [this]() { launch_system.update(); });
    update_with_timer("Move", [this]() { move_system.update(); });
    update_with_timer("Render", [this]() { render_system.update(); });
    update_with_timer("Ui", [this]() { Ui::get_instance().update(_registry); });

    _window.swap_buffers();
  }
}

void Game::process_input() {}

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
