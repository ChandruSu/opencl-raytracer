
#version 300 es

precision highp float;
precision highp int;

in vec2 uv;

uniform sampler2D tex;

out vec4 fragColor;

void main() {
  fragColor = texture(tex, uv);
}