#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components/Renderable.hpp"
#include "../graphic/Renderer.hpp"
#include "BaseSystem.hpp"

class RenderSystem : BaseSystem {
public:
  RenderSystem(
      entt::registry &registry,
      const entt::resource_cache<Program> &program_cache,
      const entt::resource_cache<Texture> &texture_cache,
      const entt::resource_cache<SpriteFrame> &sprite_frame_cache);

  void update() override;

private:
  const entt::resource_cache<Program> &_program_cache;
  const entt::resource_cache<Texture> &_texture_cache;
  const entt::resource_cache<SpriteFrame> &_sprite_frame_cache{};
  Renderer _renderer;
};