#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components/Sprite.hpp"

#include "Program.hpp"
#include "Texture.hpp"

class RenderSystem {
public:
  static RenderSystem &get_instance() {
    static RenderSystem renderSystem;
    return renderSystem;
  }

  void update(entt::registry &registry);

private:
  RenderSystem();
  ~RenderSystem();
  RenderSystem(const RenderSystem &other) = delete;
  RenderSystem &operator=(const RenderSystem &) = delete;

  void draw_sprite(Texture &texture, glm::vec2 position,
                   glm::vec2 size = glm::vec2(10.0f, 10.0f),
                   float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

  GLuint _VBO;
  GLuint _quadVAO;
};