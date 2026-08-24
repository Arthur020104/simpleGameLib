#version 460 core

in vec2 texCoord;
flat in uint textureIndex;

out vec4 FragColor;

uniform sampler2D textures[64];

void main()
{
  vec4 texColor = texture(textures[textureIndex], texCoord);

  if(texColor.a < 0.1)
    discard;
}