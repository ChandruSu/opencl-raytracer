
#version 300 es

precision highp float;
precision highp int;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texCoord;

out vec2 uv;

void main() {
  uv = texCoord;
  gl_Position = vec4(vertex, 1.0);
}