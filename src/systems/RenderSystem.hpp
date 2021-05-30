#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../components/Renderable.hpp"
#include "../graphic/Renderer.hpp"
#include "BaseSystem.hpp"

class RenderSystem : BaseSystem {
public:
  RenderSystem(entt::registry &registry, Renderer &renderer);

  void update() override;

private:
  Renderer& _renderer;
};