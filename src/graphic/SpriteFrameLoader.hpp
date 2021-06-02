#pragma once

#include <entt/entt.hpp>

#include "../graphic/SpriteFrame.hpp"

struct SpriteFrameLoader final
    : entt::resource_loader<SpriteFrameLoader, SpriteFrame> {
  std::shared_ptr<SpriteFrame>
  load(entt::id_type texture_id, Size source_size, Rect<int> frame) const {
    auto ptr = new SpriteFrame{texture_id, frame, source_size};
    return std::shared_ptr<SpriteFrame>{ptr};
  }

  std::shared_ptr<SpriteFrame> load(
      entt::id_type texture_id,
      entt::resource_handle<Texture> texture,
      Size source_size) const {
    return load(
        texture_id,
        source_size,
        Rect<int>{Point{0, 0}, Size{texture->_width, texture->_height}});
  }

  std::shared_ptr<SpriteFrame>
  load(entt::id_type texture_id, entt::resource_handle<Texture> texture) const {
    return load(texture_id, texture, Size{texture->_width, texture->_height});
  }
};