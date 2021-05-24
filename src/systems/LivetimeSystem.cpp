#include "LivetimeSystem.hpp"

#include "../components/Livetime.hpp"

void LivetimeSystem::update() {
  auto view = _registry.view<Livetime>();
  for (auto [entity, livetime] : view.each()) {
    livetime.time++;
    if (livetime.duration >= 0 && livetime.time >= livetime.duration) {
      _registry.destroy(entity);
    }
  }
}