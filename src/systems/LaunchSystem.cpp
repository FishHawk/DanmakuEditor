#include "LaunchSystem.hpp"

#include <utility>

#include "../components/Launchable.hpp"
#include "../components/Livetime.hpp"
#include "../components/Moveable.hpp"
#include "../components/Renderable.hpp"
#include "../util/debug.hpp"

void LaunchSystem::update() {
  auto view =
      _registry.view<const Livetime, const Moveable, const Launchable>();
  for (auto [parent, cycle, moveable, launchable] : view.each()) {
    auto launchable_copy = launchable;

    auto repetition =
        launchable_copy.repetition_generator(cycle.duration, cycle.time);

    for (auto i = 0u; i < repetition; ++i) {
      const auto child = _registry.create();

      _registry.emplace<Livetime>(child, Livetime{launchable_copy.duration});
      _registry.emplace<Renderable>(child, launchable_copy.sprite_frame);

      auto origin = launchable_copy.origin_generator(
          parent, moveable.position, cycle.time, i);
      auto direction = launchable_copy.direction_generator(
          atan2(moveable.offset.y, moveable.offset.x), cycle.time, i);
      auto new_moveable = Moveable{
          .origin = origin,
          .direction = direction,
          .movements = launchable_copy.template_movements};
      _registry.emplace<Moveable>(child, new_moveable);

      if (launchable_copy.launchable_generator != nullptr) {
        auto launchable = launchable_copy.launchable_generator();
        _registry.emplace<Launchable>(child, launchable);
      }
    }
  }
}