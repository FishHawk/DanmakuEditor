#include "ModifierGenerator.hpp"

namespace modifier {

inline auto generate_rotate_mat(Direction d) {
  return glm::mat2{{cos(d), sin(d)}, {-sin(d), cos(d)}};
}

Launchable::ModifierGenerator
by_position(const Vec2ExprGenerator &expr_generator) {
  return [=](auto d) {
    auto expr = expr_generator(d);
    return [=](auto p, auto t) { return expr(t); };
  };
}
Launchable::ModifierGenerator
by_velocity(const Vec2ExprGenerator &expr_generator) {
  return [=](auto d) {
    auto expr = expr_generator(d);
    return [=](auto p, auto t) { return p + expr(t); };
  };
}

Vec2ExprGenerator
cartesian(const DistanceExpr &expr_x, const DistanceExpr &expr_y) {
  return [=](auto d) {
    return [=](auto t) { return glm::vec2{expr_x(d, t), expr_y(d, t)}; };
  };
}
Vec2ExprGenerator
cartesian_rotated(const DistanceExpr &expr_x, const DistanceExpr &expr_y) {
  return [=](auto d) {
    auto rotate_mat = generate_rotate_mat(d);
    return [=](auto t) {
      return rotate_mat * glm::vec2{expr_x(d, t), expr_y(d, t)};
    };
  };
}
Vec2ExprGenerator
polar(const DistanceExpr &expr_r, const DirectionExpr &expr_phi) {
  return [=](auto d) {
    return [=](auto t) {
      auto r = expr_r(d, t);
      auto phi = expr_phi(d, t);
      return glm::vec2{cos(phi), sin(phi)} * r;
    };
  };
}
Vec2ExprGenerator
polar_rotated(const DistanceExpr &expr_r, const DirectionExpr &expr_phi) {
  return [=](Direction d) {
    return [=](Time t) {
      auto r = expr_r(d, t);
      auto phi = d + expr_phi(d, t);
      return glm::vec2{cos(phi), sin(phi)} * r;
    };
  };
}

} // namespace modifier