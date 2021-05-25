#pragma once

#include "../components/math.hpp"
#include "Texture.hpp"

struct SpriteFrame {
  const Texture &texture;
  Rect frame;
  Size source_size;
};