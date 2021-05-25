#pragma once

#include <filesystem>

#include <entt/entt.hpp>

#include "ProgramLoader.hpp"
#include "SpriteFrameLoader.hpp"
#include "TextureLoader.hpp"

class ResourceManager {
public:
  ResourceManager(const std::filesystem::path &resource_dir);

  using ProgramCache = entt::resource_cache<Program>;
  using TextureCache = entt::resource_cache<Texture>;
  using SpriteFrameCache = entt::resource_cache<SpriteFrame>;

  const ProgramCache &program_cache() const { return _program_cache; }
  const TextureCache &texture_cache() const { return _texture_cache; }
  const SpriteFrameCache &sprite_frame_cache() const {
    return _sprite_frame_cache;
  }

private:
  std::filesystem::path _resource_dir;
  ProgramCache _program_cache{};
  TextureCache _texture_cache{};
  SpriteFrameCache _sprite_frame_cache{};
};