#version 150

in vec2 uvCoord;

out vec4 fragColour;

void main()
{
  fragColour = vec4(uvCoord.x, 0.0, uvCoord.y, 1.0);
}