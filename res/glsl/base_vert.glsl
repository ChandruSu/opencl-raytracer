
#version 300 es

precision highp float;
precision highp int;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texCoord;

out vec2 uv;

uniform mat4 projection;

void main() {
  uv = texCoord;
  gl_Position = projection * vec4(vertex * 0.95, 1.0);
}