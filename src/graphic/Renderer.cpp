#include "Renderer.hpp"

#include "../util/debug.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace entt::literals;

Renderer::Renderer(
    const entt::resource_cache<Program> &program_cache,
    const entt::resource_cache<Texture> &texture_cache,
    const entt::resource_cache<SpriteFrame> &sprite_frame_cache)
    : _program_cache(program_cache), _texture_cache(texture_cache),
      _sprite_frame_cache(sprite_frame_cache) {
  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);

  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  // position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(
      1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // color attribute
  glVertexAttribPointer(
      2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(4 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

Renderer::~Renderer() {
  glDeleteVertexArrays(1, &_VAO);
  glDeleteBuffers(1, &_VBO);
}

void Renderer::push_sprite(Sprite sprite) {
  const auto frame = _sprite_frame_cache.handle(sprite.frame_id);

  auto model = glm::mat2{
      {cos(sprite.rotation), sin(sprite.rotation)},
      {-sin(sprite.rotation), cos(sprite.rotation)}};

  auto &size = frame->source_size;
  auto &position = sprite.position;
  auto &color = sprite.color;

  auto p_tl = position + model * (size * glm::vec2{-0.5, 0.5});
  auto p_tr = position + model * (size * glm::vec2{0.5, 0.5});
  auto p_bl = position + model * (size * glm::vec2{-0.5, -0.5});
  auto p_br = position + model * (size * glm::vec2{0.5, -0.5});

  auto t_tl = glm::vec2{0, 1};
  auto t_tr = glm::vec2{1, 1};
  auto t_bl = glm::vec2{0, 0};
  auto t_br = glm::vec2{1, 0};

  auto &g = _groups[frame->texture_id];
  g.emplace_back(p_tr, t_tr, color); // top right
  g.emplace_back(p_br, t_br, color); // bottom right
  g.emplace_back(p_bl, t_bl, color); // bottom left

  g.emplace_back(p_bl, t_bl, color); // bottom left
  g.emplace_back(p_tl, t_tl, color); // top right
  g.emplace_back(p_tr, t_tr, color); // top right
}

void Renderer::render() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto program = _program_cache.handle("base"_hs);
  program->use();

  for (auto &[texture_id, group] : _groups) {
    glActiveTexture(GL_TEXTURE0);
    _texture_cache.handle(texture_id)->bind();

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        group.size() * sizeof(Vertex),
        group.data(),
        GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, group.size());
    glBindVertexArray(0);
    group.clear();
  }
}