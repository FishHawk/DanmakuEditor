#include "Game.hpp"

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

Game::Game()
    : _resource_manager("/home/wh/Projects/DanmakuEditor/assets/"),
      _spell_manager(_resource_manager),
      _renderer(
          _resource_manager.program_cache(),
          _resource_manager.texture_cache(),
          _resource_manager.sprite_frame_cache()) {
  run_spell(0);

  auto &dispatcher = _window.dispatcher;
  dispatcher.sink<ResizeEvent>().connect<&Game::on_resize_event>(this);
  dispatcher.sink<KeyEvent>().connect<&Game::on_key_event>(this);
  dispatcher.sink<ScrollEvent>().connect<&Game::on_scroll_event>(this);
  _window.poll_events();
}

void Game::on_resize_event(const ResizeEvent &e) {
  auto &camera = _renderer.camera();
  camera.set_screen_size(e.width, e.height);
  camera.set_size(0.5f * e.width, 0.5f * e.height);
}

void Game::on_key_event(const KeyEvent &e) {
  if (e.key == Key::Escape && e.state == KeyState::Press)
    _window.close();
  if (e.key == Key::F1 && e.state == KeyState::Press)
    Ui::get_instance().toggle_console_window();
}

void Game::on_scroll_event(const ScrollEvent &e) {
  if (e.axis == ScrollEvent::Axis::Y) {
    auto &camera = _renderer.camera();
    camera.zoom(1 + e.offset * 0.12);
  }
}

void Game::loop() {
  static LivetimeSystem livetime_system{_registry};
  static MoveSystem move_system{_registry};
  static LaunchSystem launch_system{_registry};
  static RenderSystem render_system{_registry, _renderer};

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

    _window.display();
  }
}

void Game::process_input() {
  float speed = 5, rotate_speed = glm::radians(360.f) / 5 / 60;
  if (_window.is_key_pressed(Key::W)) {
    _renderer.camera().move(0, -speed);
  }
  if (_window.is_key_pressed(Key::S)) {
    _renderer.camera().move(0, speed);
  }
  if (_window.is_key_pressed(Key::A)) {
    _renderer.camera().move(-speed, 0);
  }
  if (_window.is_key_pressed(Key::D)) {
    _renderer.camera().move(speed, 0);
  }

  if (_window.is_key_pressed(Key::Q)) {
    _renderer.camera().rotate(rotate_speed);
  }
  if (_window.is_key_pressed(Key::E)) {
    _renderer.camera().rotate(-rotate_speed);
  }
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
