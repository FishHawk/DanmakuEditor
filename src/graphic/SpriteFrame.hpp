#pragma once

#include "../components/math.hpp"
#include "Texture.hpp"

struct SpriteFrame {
  entt::id_type texture_id;
  Rect frame;
  Size source_size;
};