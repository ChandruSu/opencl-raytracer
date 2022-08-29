
#version 300 es

precision highp float;
precision highp int;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 transformation;
uniform mat4 view;

out vec2 uv;
out vec3 norm;
out vec3 toCamera;

void main() {
  uv = texCoord;
  norm = (transformation * vec4(normal, 0.0)).xyz;

  vec4 worldPos = transformation * vec4(vertex, 1.0);
  gl_Position = projection * view * worldPos;

  toCamera = normalize(inverse(view) * vec4(0.0, 0.0, 0.0, 1.0) - worldPos).xyz;
}