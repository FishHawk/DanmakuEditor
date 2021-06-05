#include "MoveSystem.hpp"

#include <utility>

#include "../components/Launchable.hpp"
#include "../components/Livetime.hpp"
#include "../components/Moveable.hpp"
#include "../components/Renderable.hpp"
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

      auto &movement = moveable.movement;
      if (movement.descriptor != nullptr) {
        auto vec2 = movement.descriptor(moveable.direction, cycle.time);
        if (movement.mode == Movement::Mode::ByPosition) {
          moveable.offset = vec2;
        } else if (movement.mode == Movement::Mode::ByVelocity) {
          moveable.offset += vec2;
        }
      }

      moveable.position = origin + moveable.offset;
    }
  }
}