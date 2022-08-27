
#version 300 es

precision highp float;
precision highp int;

in vec2 uv;

out vec4 fragColor;

void main() {
  fragColor = vec4(uv.x, 1.0, uv.y, 1.0);
}