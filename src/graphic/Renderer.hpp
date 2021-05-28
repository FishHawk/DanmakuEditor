#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>

#include "Program.hpp"
#include "Sprite.hpp"
#include "SpriteFrame.hpp"
#include "Texture.hpp"

class Renderer {
public:
  Renderer(
      const entt::resource_cache<Program> &program_cache,
      const entt::resource_cache<Texture> &texture_cache,
      const entt::resource_cache<SpriteFrame> &sprite_frame_cache);
  ~Renderer();

  void push_sprite(Sprite sprite);

  void render();

private:
  GLuint _VBO;
  GLuint _quadVAO;

  std::vector<Sprite> _sprites;

  const entt::resource_cache<Program> &_program_cache;
  const entt::resource_cache<Texture> &_texture_cache;
  const entt::resource_cache<SpriteFrame> &_sprite_frame_cache{};
};