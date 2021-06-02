#pragma once

#include <entt/entt.hpp>

#include "../graphic/Texture.hpp"

struct TextureLoader final : entt::resource_loader<TextureLoader, Texture> {
  std::shared_ptr<Texture> load(const std::string &filename, bool alpha) const;
};