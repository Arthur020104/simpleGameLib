#version 460 core

in vec3 direction;

out vec4 FragColor;

uniform samplerCube environmentTexture;

void main()
{
  FragColor = texture(environmentTexture, direction);
}