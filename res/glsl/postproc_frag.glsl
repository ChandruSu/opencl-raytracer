
#version 300 es

precision highp float;
precision highp int;

in vec2 uv;

uniform sampler2D tex;

out vec4 fragColor;

void main() {
  // inverts colour post processing effect
  fragColor = vec4(vec3(1.0) - texture(tex, uv).rgb, 1.0);
}