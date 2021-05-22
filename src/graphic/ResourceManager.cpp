#include "ResourceManager.hpp"

#include <fstream>
#include <sstream>

#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Program> ResourceManager::programs;

Program ResourceManager::add_program(
    const std::string &name,
    const std::string &vShaderFilename,
    const std::string &fShaderFilename) {
  auto it = programs.emplace(
      name, load_program_from_file(vShaderFilename, fShaderFilename));
  return it.first->second;
}

Program ResourceManager::get_program(const std::string &name) {
  return programs.at(name);
}

Texture ResourceManager::add_texture(
    const std::string &name, const std::string &filename, bool alpha) {
  textures[name] = load_texture_from_file(filename.c_str(), alpha);
  return textures[name];
}

Texture ResourceManager::get_texture(const std::string &name) {
  return textures.at(name);
}

void ResourceManager::clear() {
  programs.clear();
  textures.clear();
}

std::string read_file(const std::string &filename) {
  std::ifstream file;
  std::stringstream file_stream;

  try {
    file.open(filename);
    file_stream << file.rdbuf();
    file.close();
  } catch (std::exception e) {
    spdlog::error("ERROR::SHADER: Failed to read shader files");
  }

  return file_stream.str();
}

Program ResourceManager::load_program_from_file(
    const std::string &vShaderFilename, const std::string &fShaderFilename) {
  return Program{read_file(vShaderFilename), read_file(fShaderFilename)};
}

Texture ResourceManager::load_texture_from_file(const char *file, bool alpha) {
  Texture texture;
  if (alpha) {
    texture._internal_format = GL_RGBA;
    texture._image_format = GL_RGBA;
  }

  int width, height, nrChannels;
  unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
  texture.generate(width, height, data);
  stbi_image_free(data);
  return texture;
}