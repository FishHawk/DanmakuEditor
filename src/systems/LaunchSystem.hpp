#pragma once

#include <entt/entt.hpp>

#include "../components/Launchable.hpp"

class LaunchSystem {
public:
  static LaunchSystem &get_instance() {
    static LaunchSystem launchSystem;
    return launchSystem;
  }

  void update(entt::registry &registry);

private:
  LaunchSystem() = default;
  ~LaunchSystem() = default;
  LaunchSystem(const LaunchSystem &other) = delete;
  LaunchSystem &operator=(const LaunchSystem &) = delete;
};
