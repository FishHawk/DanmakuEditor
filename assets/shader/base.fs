#version 330 core

in vec2 a_tex_coords;
in vec4 a_color;

out vec4 color;

uniform sampler2D image;

void main() {
  vec4 texColor = a_color * texture(image, a_tex_coords);
  if (texColor.r == 0 && texColor.g == 0 && texColor.b == 0)
    discard;
  color = texColor;
}