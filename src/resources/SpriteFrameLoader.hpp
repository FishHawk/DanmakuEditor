#pragma once

#include <entt/entt.hpp>

#include "../graphic/SpriteFrame.hpp"

struct SpriteFrameLoader final
    : entt::resource_loader<SpriteFrameLoader, SpriteFrame> {
  std::shared_ptr<SpriteFrame> load(
      entt::resource_handle<Texture> texture,
      Size source_size,
      Rect frame) const {
    auto ptr = new SpriteFrame{texture, frame, source_size};
    return std::shared_ptr<SpriteFrame>{ptr};
  }

  std::shared_ptr<SpriteFrame>
  load(entt::resource_handle<Texture> texture, Size source_size) const {
    return load(
        texture,
        source_size,
        Rect{Point{0, 0}, Size{texture->_width, texture->_height}});
  }

  std::shared_ptr<SpriteFrame>
  load(entt::resource_handle<Texture> texture) const {
    return load(texture, Size{texture->_width, texture->_height});
  }
};