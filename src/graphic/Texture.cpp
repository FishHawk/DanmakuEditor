#include <iostream>

#include "Texture.hpp"

Texture::Texture()
    : _width(0), _height(0), _internal_format(GL_RGB), _image_format(GL_RGB),
      Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Filter_Min(GL_NEAREST),
      Filter_Max(GL_NEAREST) {
  glGenTextures(1, &this->_id);
}

void Texture::generate(GLuint width, GLuint height, unsigned char *data) {
  this->_width = width;
  this->_height = height;
  // Create Texture
  glBindTexture(GL_TEXTURE_2D, this->_id);
  glTexImage2D(GL_TEXTURE_2D, 0, this->_internal_format, width, height, 0,
               this->_image_format, GL_UNSIGNED_BYTE, data);
  // Set Texture wrap and filter modes
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
  // Unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, this->_id); }