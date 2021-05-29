#pragma once

#include "../components/math.hpp"
#include "Texture.hpp"

struct SpriteFrame {
  Texture &texture;
  Rect frame;
  Size source_size;
};