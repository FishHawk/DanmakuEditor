#include "Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "ProgramLoader.hpp"
#include "SpriteFrameLoader.hpp"
#include "TextureLoader.hpp"

using namespace entt::literals;

Renderer::Renderer() {
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

void Renderer::load_resource(std::filesystem::path dir) {
  const auto program_dir = dir / "shader";
  const auto texture_dir = dir / "texture";

  _program_cache.load<ProgramLoader>(
      "base"_hs, program_dir / "base.vs", program_dir / "base.fs");

  {
    auto texture = _texture_cache.load<TextureLoader>(
        "circle"_hs, texture_dir / "circle.png", true);
    _sprite_frame_cache.load<SpriteFrameLoader>(
        "aircraft"_hs, "circle"_hs, texture);
    _sprite_frame_cache.load<SpriteFrameLoader>(
        "cannon"_hs, "circle"_hs, texture, Size{16, 16});
  }

  {
    auto texture = _texture_cache.load<TextureLoader>(
        "bullet"_hs, texture_dir / "bullet.png", true);
    _sprite_frame_cache.load<SpriteFrameLoader>(
        "bullet"_hs, "bullet"_hs, texture);
  }
}

void Renderer::render() {
  auto screen_size = _camera.screen_size();
  glViewport(0, 0, screen_size.x, screen_size.y);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto program = _program_cache.handle("base"_hs);
  program->use();
  program->set_mat4("projection", _camera.transform());
  program->set_int("image", 0);

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