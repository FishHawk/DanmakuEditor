#pragma once

#include <glm/glm.hpp>

#include "base.hpp"

template <typename T>
class Rect {
public:
  Rect() : Rect(0, 0, 0, 0) {}
  Rect(T x, T y, T width, T height)
      : _x(x), _y(y), _width(width), _height(height){};
  Rect(const Vec2<T> &position, const Vec2<T> &size)
      : Rect(position.x, position.y, size.x, size.y){};

  Vec2<T> position() const { return Vec2<T>{_x, _y}; }
  Vec2<T> size() const { return Vec2<T>{_width, _height}; }

  T x() const { return _x; }
  T y() const { return _y; }

  T width() const { return _width; }
  T height() const { return _height; }

  T top() const { return _y; }
  T bottom() const { return _y + height; }
  T left() const { return _x; }
  T right() const { return _x + width; }

  Vec2<T> bottom_left() const { return {left(), bottom()}; }
  Vec2<T> bottom_right() const { return {right(), bottom()}; }
  Vec2<T> top_left() const { return {left(), top()}; }
  Vec2<T> top_right() const { return {right(), top()}; }
  Vec2<T> center() const { return {_x + _width / (T)2, _y + _height / (T)2}; }

private:
  T _x;
  T _y;
  T _width;
  T _height;
};
