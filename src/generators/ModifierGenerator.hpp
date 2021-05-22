#pragma once

#include "../components/Launchable.hpp"

namespace modifier {

using DistanceExpr = std::function<Distance(Direction, Time)>;
using DirectionExpr = std::function<Direction(Direction, Time)>;

using Vec2ExprGenerator =
    std::function<std::function<glm::vec2(Time)>(Direction)>;

Launchable::ModifierGenerator by_position(const Vec2ExprGenerator &expr_p);
Launchable::ModifierGenerator by_velocity(const Vec2ExprGenerator &expr_p);

Vec2ExprGenerator
cartesian(const DistanceExpr &expr_x, const DistanceExpr &expr_y);
Vec2ExprGenerator
cartesian_rotated(const DistanceExpr &expr_x, const DistanceExpr &expr_y);

Vec2ExprGenerator
polar(const DistanceExpr &expr_r, const DirectionExpr &expr_phi);
Vec2ExprGenerator
polar_rotated(const DistanceExpr &expr_r, const DirectionExpr &expr_phi);

} // namespace modifier
