#pragma once

#include <functional>
#include <variant>

#include <entt/entt.hpp>

#include "./math.hpp"

using Position = glm::vec2;
using Velocity = glm::vec2;
using Entity = entt::registry::entity_type;

struct Moveable {
  using Origin = std::variant<Position, Entity>;
  using Modifier = std::function<Position(Position, Time)>;

  Origin origin;
  Modifier modifier = nullptr;
  Position offset = Position{0.f, 0.f};
  Position position = Position{0.f, 0.f};
};
