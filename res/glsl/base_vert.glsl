
#version 300 es

precision highp float;
precision highp int;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 uv;

void main() {
  uv = texCoord;
  gl_Position = vec4(0.9 * vertex + vec3(0, 0, -1.0), 1.0);
}