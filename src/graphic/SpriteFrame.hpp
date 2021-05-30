#pragma once

#include "../util/Rect.hpp"
#include "Texture.hpp"

struct SpriteFrame {
  entt::id_type texture_id;
  Rect<int> frame;
  Size source_size;
};