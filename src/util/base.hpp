#pragma once

#include <functional>

#include <glm/glm.hpp>

template <typename T>
using Vec2 = glm::vec<2, T, glm::defaultp>;

using Point = Vec2<float>;
using Size = Vec2<float>;
using Time = int;
using Direction = float;
using Distance = float;

template <typename T>
using Expr = std::function<T(float)>;
