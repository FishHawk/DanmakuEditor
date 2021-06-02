#pragma once

#include <filesystem>

#include <entt/entt.hpp>
#include <glad/glad.h>

#include "Camera.hpp"
#include "Program.hpp"
#include "Sprite.hpp"
#include "SpriteFrame.hpp"
#include "Texture.hpp"

struct Vertex {
  glm::vec2 position;
  glm::vec2 tex_coords;
  glm::vec4 color;
};

class Renderer {
public:
  Renderer();
  ~Renderer();

  void push_sprite(Sprite sprite);

  void render();

  Camera &camera() { return _camera; }

  void load_resource(std::filesystem::path dir);

private:
  GLuint _VBO;
  GLuint _VAO;

  std::map<entt::id_type, std::vector<Vertex>> _groups;

  Camera _camera;

  entt::resource_cache<Program> _program_cache;
  entt::resource_cache<Texture> _texture_cache;
  entt::resource_cache<SpriteFrame> _sprite_frame_cache;
};