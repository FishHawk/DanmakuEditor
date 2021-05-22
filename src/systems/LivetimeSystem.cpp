#include "LivetimeSystem.hpp"

void LivetimeSystem::update(entt::registry &registry) {
  auto view = registry.view<Livetime>();
  for (auto [entity, livetime] : view.each()) {
    livetime.time++;
    if (livetime.duration >= 0 && livetime.time >= livetime.duration) {
      registry.destroy(entity);
    }
  }
}