#pragma once

#include "../components/Launchable.hpp"

namespace origin {

using PositionExpr = std::function<Position(Position, Time, size_t)>;

Launchable::OriginGenerator at();
Launchable::OriginGenerator at(Position p);
Launchable::OriginGenerator at(PositionExpr expr);
// Launchable::OriginGenerator at_player();

Launchable::OriginGenerator follow();
Launchable::OriginGenerator follow(Entity e);
// Launchable::OriginGenerator follow_player();

} // namespace origin
