#pragma once

#include <glad/glad.h>

class Texture {
public:
  Texture();
  void generate(GLuint width, GLuint height, unsigned char *data);
  void bind() const;

public:
  GLuint _id;
  GLuint _width, _height;
  GLuint _internal_format;
  GLuint _image_format;

  GLuint Wrap_S;     // Wrapping mode on S axis
  GLuint Wrap_T;     // Wrapping mode on T axis
  GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
  GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
};