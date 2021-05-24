#include "ResourceManager.hpp"

using namespace entt::literals;

ResourceManager::ResourceManager(const std::filesystem::path &resource_dir)
    : _resource_dir(resource_dir) {
  const auto program_dir = _resource_dir / "shader";
  const auto texture_dir = _resource_dir / "texture";

  _program_cache.load<ProgramLoader>(
      "base"_hs, program_dir / "base.vs", program_dir / "base.fs");

  _texture_cache.load<TextureLoader>(
      "circle"_hs, texture_dir / "circle.png", true);
  _texture_cache.load<TextureLoader>(
      "bullet"_hs, texture_dir / "bullet.png", true);
}