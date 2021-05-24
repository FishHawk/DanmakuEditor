#pragma once

#include <entt/entt.hpp>

class BaseSystem {
public:
  BaseSystem(entt::registry &registry) : _registry(registry){};

  virtual void update() = 0;

protected:
  BaseSystem(const BaseSystem &other) = delete;
  BaseSystem &operator=(const BaseSystem &) = delete;

  entt::registry &_registry;
};