#include "MoveSystem.hpp"

#include <utility>

#include "../components/Moveable.hpp"

void update_moveable_offset(Moveable &obj, const Movement &movement) {
  if (movement.descriptor != nullptr) {
    auto vec2 = movement.descriptor(obj.direction, obj.livetime);
    if (movement.is_additional) {
      obj.offset += vec2;
    } else {
      obj.offset = vec2;
    }
  }
}

void MoveSystem::update() {
  auto view = _registry.view<Moveable>();
  for (auto [entity, moveable] : view.each()) {
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

    if (!moveable.movements.empty()) {
      update_moveable_offset(moveable, moveable.movements.front());
    }

    moveable.livetime++;
    moveable.position_buf = origin + moveable.offset;
  }

  for (auto [entity, moveable] : view.each()) {
    moveable.position = moveable.position_buf;
  }
}