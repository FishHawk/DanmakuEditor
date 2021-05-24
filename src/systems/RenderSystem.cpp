#include "RenderSystem.hpp"

#include "../graphic/Program.hpp"
#include "../graphic/ResourceManager.hpp"

RenderSystem::RenderSystem(entt::registry &registry) : BaseSystem(registry) {
  float vertices[] = {
      // pos      // tex
      0.0f, 1.0f, 0.0f, 1.0f, //
      1.0f, 0.0f, 1.0f, 0.0f, //
      0.0f, 0.0f, 0.0f, 0.0f, //

      0.0f, 1.0f, 0.0f, 1.0f, //
      1.0f, 1.0f, 1.0f, 1.0f, //
      1.0f, 0.0f, 1.0f, 0.0f  //
  };

  glGenVertexArrays(1, &this->_quadVAO);
  glGenBuffers(1, &_VBO);

  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(this->_quadVAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

RenderSystem::~RenderSystem() {
  glDeleteVertexArrays(1, &_quadVAO);
  glDeleteBuffers(1, &_VBO);
}

void RenderSystem::draw_sprite(
    Texture &texture,
    glm::vec2 position,
    glm::vec2 size,
    float rotate,
    glm::vec3 color) {
  auto program = ResourceManager::get_program("base");
  program.use();

  glm::mat4 model = glm::mat4(1.0f);

  // first translate (transformations are: scale happens first, then rotation,
  // and then final translation happens; reversed order)
  model = glm::translate(model, glm::vec3(position, 0.0f));
  // model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y,
  // 0.0f));
  model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
  model =
      glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
  model = glm::scale(model, glm::vec3(size, 1.0f));

  program.set_mat4("model", model);
  program.set_vec3("spriteColor", color);

  glActiveTexture(GL_TEXTURE0);
  texture.bind();

  glBindVertexArray(_quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void RenderSystem::update() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  auto view = _registry.view<const Sprite>();
  for (auto [entity, sprite] : view.each()) {
    if (sprite.type == 0) {
      auto texture = ResourceManager::get_texture("circle");
      draw_sprite(
          texture, sprite.position, sprite.size, sprite.rotate, sprite.color);
    } else if (sprite.type == 1) {
      auto texture = ResourceManager::get_texture("bullet");
      draw_sprite(
          texture, sprite.position, sprite.size, sprite.rotate, sprite.color);
    }
  }
}