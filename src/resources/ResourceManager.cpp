#include "ResourceManager.hpp"

using namespace entt::literals;

ResourceManager::ResourceManager(const std::filesystem::path &resource_dir)
    : _resource_dir(resource_dir) {
  const auto program_dir = _resource_dir / "shader";
  const auto texture_dir = _resource_dir / "texture";

  _program_cache.load<ProgramLoader>(
      "base"_hs, program_dir / "base.vs", program_dir / "base.fs");

  {
    auto texture = _texture_cache.load<TextureLoader>(
        "circle"_hs, texture_dir / "circle.png", true);
    _sprite_frame_cache.load<SpriteFrameLoader>(
        "aircraft"_hs, "circle"_hs, texture);
    _sprite_frame_cache.load<SpriteFrameLoader>(
        "cannon"_hs, "circle"_hs, texture, Size{16, 16});
  }

  {
    auto texture = _texture_cache.load<TextureLoader>(
        "bullet"_hs, texture_dir / "bullet.png", true);
    _sprite_frame_cache.load<SpriteFrameLoader>(
        "bullet"_hs, "bullet"_hs, texture);
  }
}