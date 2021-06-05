#pragma once

#include <functional>
#include <variant>

#include <entt/entt.hpp>

#include "../util/base.hpp"

using Position = Vec2<Distance>;
using Entity = entt::registry::entity_type;

struct Movement {
  bool is_additional;
  std::function<Position(Direction, Time)> descriptor;
};

struct Moveable {
  using Origin = std::variant<Position, Entity>;
  using Modifier = std::function<Position(Position, Time)>;

  Origin origin;
  Direction direction{0};
  Time livetime{0};
  Movement movement{};
  Position offset{0.f, 0.f};
  Position position{0.f, 0.f};
  Position position_buf{0.f, 0.f};
};
