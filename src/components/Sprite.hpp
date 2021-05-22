#pragma once

#include "../graphic/Texture.hpp"
#include "Moveable.hpp"

struct Sprite {
  // Texture texture;
  int type;
  Position position;
  Size size;
  float rotate;
  glm::vec3 color;
};