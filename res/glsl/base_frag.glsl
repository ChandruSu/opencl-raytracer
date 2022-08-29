
#version 300 es

precision highp float;
precision highp int;

in vec2 uv;
in vec3 norm;
in vec3 toCamera;

out vec4 fragColor;

const vec3 toLight = normalize(vec3(1.0, 5.0, 3.0));

void main() {
  float diffuse_lighting = max(dot(norm, toLight), 0.05);
  float specular_lighting = dot(reflect(-toLight, norm), toCamera);
  specular_lighting = 1.9 * pow(max(specular_lighting, 0.0), 25.0);
  fragColor = vec4(vec3(0.0, 0.0, 1.0) * diffuse_lighting + vec3(specular_lighting), 1.0);
}