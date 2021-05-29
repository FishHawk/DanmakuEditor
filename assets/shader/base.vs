#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 tex_coords;
layout(location = 2) in vec4 color;

out vec2 a_tex_coords;
out vec4 a_color;

uniform mat4 projection;

void main() {
  a_color = color;
  a_tex_coords = tex_coords;
  gl_Position = projection * vec4(position, 0.0, 1.0);
}