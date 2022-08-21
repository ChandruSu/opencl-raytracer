
#version 300 es

#define NEAR_PLANE 0.001
#define FAR_PLANE 1000.0

precision highp float;
precision highp int;

in vec2 uv;

uniform sampler2D tex;
uniform sampler2D dep;

out vec4 fragColor;

void main() {
  // inverts colour post processing effect
  vec4 colour = texture(tex, uv);
  float depth = texture(dep, uv).r;
  float floorDist = (2.0 * NEAR_PLANE * FAR_PLANE) / (FAR_PLANE + NEAR_PLANE - (2.0 * depth - 1.0) * (FAR_PLANE - NEAR_PLANE));
  fragColor = vec4(floorDist/4.0);
}