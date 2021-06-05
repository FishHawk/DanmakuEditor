#pragma once

#include <functional>
#include <variant>

#include <entt/entt.hpp>

#include "../util/base.hpp"

using Position = Vec2<Distance>;
using Entity = entt::registry::entity_type;

struct Movement {
  Time duration{std::numeric_limits<Time>::max()};
  bool is_additional;
  std::function<Position(Direction, Time)> descriptor;
};

struct Moveable {
  using Origin = std::variant<Position, Entity>;

  Origin origin{Position{0.f, 0.f}};
  Direction direction{0};
  std::vector<Movement> movements{};
  Time livetime{0};
  Position offset{0.f, 0.f};
  Position position{0.f, 0.f};
  Position position_buf{0.f, 0.f};
};
