#include "Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace entt::literals;

Renderer::Renderer(
    const entt::resource_cache<Program> &program_cache,
    const entt::resource_cache<Texture> &texture_cache,
    const entt::resource_cache<SpriteFrame> &sprite_frame_cache)
    : _program_cache(program_cache), _texture_cache(texture_cache),
      _sprite_frame_cache(sprite_frame_cache) {

  float vertices[] = {
      // pos      // tex
      -0.5f, 0.5f,  0.0f, 1.0f, //
      0.5f,  -0.5f, 1.0f, 0.0f, //
      -0.5f, -0.5f, 0.0f, 0.0f, //

      -0.5f, 0.5f,  0.0f, 1.0f, //
      0.5f,  0.5f,  1.0f, 1.0f, //
      0.5f,  -0.5f, 1.0f, 0.0f  //
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

Renderer::~Renderer() {
  glDeleteVertexArrays(1, &_quadVAO);
  glDeleteBuffers(1, &_VBO);
}

void Renderer::push_sprite(Sprite sprite) { _sprites.push_back(sprite); }

void Renderer::render() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (const auto sprite : _sprites) {
    const auto frame = _sprite_frame_cache.handle(sprite.frame_id);
    frame->texture.bind();

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(sprite.position, 0.0f));
    model = glm::rotate(
        model, glm::radians(sprite.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(frame->source_size, 1.0f));

    auto program = _program_cache.handle("base"_hs);
    program->use();
    program->set_mat4("model", model);
    program->set_vec3("spriteColor", sprite.color);

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
  }

  _sprites.clear();
}