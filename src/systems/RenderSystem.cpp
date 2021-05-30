#include "RenderSystem.hpp"

using namespace entt::literals;

RenderSystem::RenderSystem(entt::registry &registry, Renderer &renderer)
    : BaseSystem(registry), _renderer(renderer) {}

void RenderSystem::update() {
  auto view = _registry.view<const Renderable>();
  for (auto [entity, renderable] : view.each()) {
    Position position{0, 0};

    auto moveable = _registry.try_get<Moveable>(entity);
    if (moveable != nullptr) {
      position = moveable->position;
    }

    _renderer.push_sprite(Sprite{
        renderable.frame, position, glm::vec2{1, 1}, 0, glm::vec4{1, 1, 1, 1}});
  }

  _renderer.render();
}