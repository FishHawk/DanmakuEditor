#include "TextureLoader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::shared_ptr<Texture>
TextureLoader::load(const std::string &filename, bool alpha) const {
  Texture *texture = new Texture{};
  if (alpha) {
    texture->_internal_format = GL_RGBA;
    texture->_image_format = GL_RGBA;
  }

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
  texture->generate(width, height, data);
  stbi_image_free(data);

  return std::shared_ptr<Texture>{texture};
}
