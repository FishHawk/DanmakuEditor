#pragma once

#include "../graphic/SpriteFrame.hpp"
#include "Moveable.hpp"

struct Launchable {
  using RepetitionGenerator = std::function<size_t(const Time, const Time)>;
  using OriginGenerator =
      std::function<Moveable::Origin(Entity, Position, Time, size_t)>;
  using DirectionGenerator = std::function<Direction(Direction, Time, size_t)>;
  using ModifierGenerator = std::function<Moveable::Modifier(Direction)>;
  using LaunchableGenerator = std::function<Launchable()>;

  entt::resource_handle<SpriteFrame> sprite_type;
  Time duration;
  RepetitionGenerator repetition_generator;
  OriginGenerator origin_generator;
  DirectionGenerator direction_generator;
  ModifierGenerator modifier_generator;
  LaunchableGenerator launchable_generator = nullptr;
};