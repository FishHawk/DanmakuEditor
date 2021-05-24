#pragma once

#include "BaseSystem.hpp"

class LaunchSystem : BaseSystem {
public:
  LaunchSystem(entt::registry &registry) : BaseSystem(registry){};
  void update() override;
};
