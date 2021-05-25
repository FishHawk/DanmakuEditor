#pragma once

#include "../graphic/Texture.hpp"
#include "Moveable.hpp"

struct Sprite {
  Texture const *texture;
  Position position;
  Size size;
  float rotate;
  glm::vec3 color;
};