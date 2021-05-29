#include "RenderSystem.hpp"

using namespace entt::literals;

RenderSystem::RenderSystem(
    entt::registry &registry,
    const entt::resource_cache<Program> &program_cache,
    const entt::resource_cache<Texture> &texture_cache,
    const entt::resource_cache<SpriteFrame> &sprite_frame_cache)
    : BaseSystem(registry),
      _renderer(program_cache, texture_cache, sprite_frame_cache),
      _program_cache(program_cache), _texture_cache(texture_cache),
      _sprite_frame_cache(sprite_frame_cache) {}

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