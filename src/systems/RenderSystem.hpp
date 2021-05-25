#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components/Sprite.hpp"
#include "../graphic/Program.hpp"
#include "../graphic/Texture.hpp"
#include "BaseSystem.hpp"

class RenderSystem : BaseSystem {
public:
  RenderSystem(
      entt::registry &registry,
      const entt::resource_cache<Program> &program_cache,
      const entt::resource_cache<Texture> &texture_cache);
  ~RenderSystem();

  void update() override;

private:
  void draw_sprite(
      const Texture &texture,
      glm::vec2 position,
      glm::vec2 size = glm::vec2(10.0f, 10.0f),
      float rotate = 0.0f,
      glm::vec3 color = glm::vec3(1.0f));

  GLuint _VBO;
  GLuint _quadVAO;

  const entt::resource_cache<Program> &_program_cache;
  const entt::resource_cache<Texture> &_texture_cache;
};