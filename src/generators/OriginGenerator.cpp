#include "OriginGenerator.hpp"

namespace origin {

Launchable::OriginGenerator at() {
  return [](auto, auto p, auto, auto) { return p; };
}
Launchable::OriginGenerator at(Position p) {
  return [=](auto, auto, auto, auto) { return p; };
}
Launchable::OriginGenerator at(PositionExpr expr) {
  return [=](auto, auto p, auto t, auto i) { return expr(p, t, i); };
}

Launchable::OriginGenerator follow(Entity e) {
  return [=](auto, auto, auto, auto) { return e; };
}
Launchable::OriginGenerator follow() {
  return [](auto e, auto, auto, auto) { return e; };
}

} // namespace origin