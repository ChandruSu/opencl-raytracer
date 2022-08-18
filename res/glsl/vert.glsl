#version 150

in vec3 pos;
in vec2 uv;

out vec2 uvCoord;

void main() {
  uvCoord = uv;
  gl_Position = vec4(pos, 1.0);
}