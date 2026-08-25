#version 460 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D textures[64];

flat in uint textureIndex;

void main()
{
  vec4 texColor = texture(textures[textureIndex], texCoord);
  if(texColor.a < 0.1)
    discard;

  FragColor = texColor; 
}