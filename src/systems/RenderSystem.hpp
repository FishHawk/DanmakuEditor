#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components/Sprite.hpp"
#include "../graphic/Texture.hpp"
#include "BaseSystem.hpp"

class RenderSystem : BaseSystem {
public:
  RenderSystem(entt::registry &registry);
  ~RenderSystem();

  void update() override;

private:
  void draw_sprite(
      Texture &texture,
      glm::vec2 position,
      glm::vec2 size = glm::vec2(10.0f, 10.0f),
      float rotate = 0.0f,
      glm::vec3 color = glm::vec3(1.0f));

  GLuint _VBO;
  GLuint _quadVAO;
};