#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main() {
  // color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
  color = texture(image, TexCoords);
  vec4 texColor = texture(image, TexCoords);
  if (texColor.r == 0 && texColor.g == 0 && texColor.b == 0)
    discard;
  color = texColor;
}