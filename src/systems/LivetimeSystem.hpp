#pragma once

#include <entt/entt.hpp>

#include "../components/Livetime.hpp"

class LivetimeSystem {
public:
  static LivetimeSystem &get_instance() {
    static LivetimeSystem livetimeSystem;
    return livetimeSystem;
  }

  void update(entt::registry &registry);

private:
  LivetimeSystem() = default;
  ~LivetimeSystem() = default;
  LivetimeSystem(const LivetimeSystem &other) = delete;
  LivetimeSystem &operator=(const LivetimeSystem &) = delete;
};
