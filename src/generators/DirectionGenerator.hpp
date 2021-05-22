#pragma once

#include "../components/Launchable.hpp"

namespace direction {

using DirectionExpr = std::function<Direction(Direction, Time, size_t)>;

Launchable::DirectionGenerator at();
Launchable::DirectionGenerator at(Direction d);
Launchable::DirectionGenerator at(DirectionExpr expr);

Launchable::DirectionGenerator
circle(Direction center, Direction range, size_t number);

} // namespace direction
