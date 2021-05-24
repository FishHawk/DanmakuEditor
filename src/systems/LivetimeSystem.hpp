#pragma once

#include "BaseSystem.hpp"

class LivetimeSystem : BaseSystem {
public:
  LivetimeSystem(entt::registry &registry) : BaseSystem(registry){};
  void update() override;
};
