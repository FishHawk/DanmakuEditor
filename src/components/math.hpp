#pragma once

#include <functional>

#include <glm/glm.hpp>

using Point = glm::vec2;
using Size = glm::vec2;
using Time = int;
using Direction = float;
using Distance = float;

struct Rect {
  Point origin;
  Size size;

  float min_x() const { return origin.x; }
  float mid_x() const { return origin.x + 0.5 * size.x; }
  float max_x() const { return origin.x + size.x; }
  float min_y() const { return origin.y; }
  float mid_y() const { return origin.y + 0.5 * size.y; }
  float max_y() const { return origin.y + size.y; }
};

template <typename T>
using Expr = std::function<T(float)>;
