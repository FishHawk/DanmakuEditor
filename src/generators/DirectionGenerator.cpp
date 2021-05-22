#include "DirectionGenerator.hpp"

namespace direction {

Launchable::DirectionGenerator at() {
  return [](auto d, auto, auto) { return d; };
}
Launchable::DirectionGenerator at(Direction d) {
  return [=](auto, auto, auto) { return d; };
}
Launchable::DirectionGenerator at(DirectionExpr expr) {
  return [=](auto d, auto t, auto i) { return expr(d, t, i); };
}

Launchable::DirectionGenerator
circle(Direction center, Direction range, size_t number) {
  return [=](auto d, auto t, auto i) {
    return center + range * ((i + 0.5f) / number - 0.5);
  };
}

} // namespace direction
