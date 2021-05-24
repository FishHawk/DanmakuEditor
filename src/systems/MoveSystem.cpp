#include "MoveSystem.hpp"

#include <utility>

#include "../components/Launchable.hpp"
#include "../components/Livetime.hpp"
#include "../components/Moveable.hpp"
#include "../components/Sprite.hpp"
#include "../util/debug.hpp"

void MoveSystem::update() {
  {
    auto view = _registry.view<const Livetime, Moveable>();
    for (auto [entity, cycle, moveable] : view.each()) {
      const auto origin = std::visit(
          [&](Moveable::Origin &&arg) -> Position {
            if (std::holds_alternative<Position>(arg)) {
              return std::get<Position>(arg);
            } else if (std::holds_alternative<Entity>(arg)) {
              const auto parent = std::get<Entity>(arg);
              return _registry.get<Moveable>(parent).position;
            } else {
              return Position{0.f, 0.f};
            }
          },
          moveable.origin);
      if (moveable.modifier != nullptr) {
        moveable.offset = moveable.modifier(moveable.offset, cycle.time);
      }
      moveable.position = origin + moveable.offset;
    }
  }

  {
    auto view = _registry.view<Sprite, const Moveable>();
    for (auto [entity, sprite, moveable] : view.each()) {
      sprite.position = moveable.position;
    }
  }
}