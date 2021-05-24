#pragma once

#include "BaseSystem.hpp"

class MoveSystem : BaseSystem {
public:
  MoveSystem(entt::registry &registry) : BaseSystem(registry){};
  void update() override;
};