#include "LaunchSystem.hpp"

#include <utility>

#include "../components/Livetime.hpp"
#include "../components/Moveable.hpp"
#include "../components/Sprite.hpp"
#include "../util/debug.hpp"

static const std::vector<Sprite> sprites{
    Sprite{0, Position{0.f, 0.f}, Size{64.f, 64.f}, 0},
    Sprite{0, Position{0.f, 0.f}, Size{16.f, 16.f}, 0},
    Sprite{1, Position{0.f, 0.f}, Size{10.f, 10.f}, 0},
};

void LaunchSystem::update(entt::registry &registry) {
  auto view = registry.view<const Livetime, const Moveable, const Launchable>();
  for (auto [parent, cycle, moveable, launchable] : view.each()) {
    auto launchable_copy = launchable;

    auto repetition =
        launchable_copy.repetition_generator(cycle.duration, cycle.time);

    for (auto i = 0u; i < repetition; ++i) {
      const auto child = registry.create();

      registry.emplace<Livetime>(child, Livetime{launchable_copy.duration});

      if (launchable_copy.sprite_type >= 0) {
        registry.emplace<Sprite>(child, sprites[launchable_copy.sprite_type]);
      }

      auto origin = launchable_copy.origin_generator(
          parent, moveable.position, cycle.time, i);
      if (launchable_copy.modifier_generator != nullptr) {
        auto direction = launchable_copy.direction_generator(
            atan2(moveable.offset.y, moveable.offset.x), cycle.time, i);
        auto modifier = launchable_copy.modifier_generator(direction);
        registry.emplace<Moveable>(child, origin, modifier);
      } else {
        registry.emplace<Moveable>(child, origin);
      }

      if (launchable_copy.launchable_generator != nullptr) {
        auto launchable = launchable_copy.launchable_generator();
        registry.emplace<Launchable>(child, launchable);
      }
    }
  }
}