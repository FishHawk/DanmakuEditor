#pragma once

#include <entt/entt.hpp>

#include "../components/Launchable.hpp"
#include "../generators/ModifierGenerator.hpp"

class MoveSystem {
public:
  static MoveSystem &get_instance() {
    static MoveSystem moveSystem;
    return moveSystem;
  }

  void update(entt::registry &registry);

private:
  MoveSystem() = default;
  ~MoveSystem() = default;
  MoveSystem(const MoveSystem &other) = delete;
  MoveSystem &operator=(const MoveSystem &) = delete;
};