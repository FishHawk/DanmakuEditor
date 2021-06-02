#pragma once

#include <entt/entt.hpp>

#include "SpellManager.hpp"
#include "graphic/Renderer.hpp"
#include "window/Window.hpp"

class Game {
public:
  static Game &get_instance() {
    static Game game;
    return game;
  }

  void loop();

  void run_spell(size_t index);
  const auto &spell_manager() { return _spell_manager; }

private:
  Game();
  ~Game() = default;
  Game(const Game &other) = delete;
  Game &operator=(const Game &) = delete;

  void process_input();
  void update_with_timer(
      const std::string &name, std::function<void(void)> callback);
  void render_ui();

  Window _window{600, 800, "Danmaku Editor"};
  Renderer _renderer;

  entt::registry _registry;

  SpellManager _spell_manager;
};