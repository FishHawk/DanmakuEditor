#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include "../util/base.hpp"

class Camera {
public:
  const Vec2i &screen_size() const { return _screen_size; }
  const Mat4f &transform() const {
    if (!_is_transform_updated) {
      // Mat3f view = Mat3f(1.0f);
      // view = glm::translate(view, _center);
      // view = glm::rotate(view, _rotation);

      Mat4f view = Mat4f(1.0f);
      view = glm::translate(view, glm::vec3{_center, 0});
      // view = glm::rotate(view, _rotation);

      Mat4f proj =
          glm::ortho(-_size.x, _size.x, _size.y, -_size.y, -1.0f, 1.0f);
      _transform = proj * Mat4f(view);

      _is_transform_updated = true;
    }
    return _transform;
  }

  void set_screen_size(Vec2i screen_size) { _screen_size = screen_size; }
  void set_screen_size(int width, int height) {
    set_screen_size(Vec2i{width, height});
  }

  void set_center(Vec2f center) {
    _center = center;
    _is_transform_updated = false;
  }
  void set_center(float x, float y) { set_center(Vec2f{x, y}); }

  void set_size(Vec2f size) {
    _size = size;
    _is_transform_updated = false;
  }
  void set_size(float width, float height) { set_size(Vec2f{width, height}); }

  void set_rotation(float angle) {
    _rotation = angle;
    _is_transform_updated = false;
  }

  void move(float dx, float dy) { set_center(_center.x + dx, _center.y + dy); }
  void move(const Vec2f &offset) { set_center(_center + offset); }

  void zoom(float factor) { set_size(_size.x * factor, _size.y * factor); }

  void rotate(float angle) { set_rotation(_rotation + angle); }

  void reset() {
    _center.x = 0;
    _center.y = 0;
    _rotation = 0;
  }

private:
  Vec2i _screen_size;
  Vec2f _center;
  Vec2f _size;
  float _rotation;

  mutable Mat4f _transform;
  mutable bool _is_transform_updated = false;
};