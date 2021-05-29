#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

struct Sprite {
  entt::id_type frame_id;
  glm::vec2 position;
  glm::vec2 size;
  float rotation;
  glm::vec4 color;
};