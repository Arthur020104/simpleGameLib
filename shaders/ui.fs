#version 460 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D textures[1];

uniform int textureUnit;

void main()
{
  vec4 texColor = texture(textures[textureUnit], texCoord);
  if(texColor.a < 0.1)
    discard;

  FragColor = texColor; 
}