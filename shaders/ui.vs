#version 460 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec2 texCoord;

uniform mat4 modelMatrix;

void main()
{
  vec4 pos = modelMatrix * vec4(vertexPos.xyz, 1.0);

  texCoord = uv;
  gl_Position = pos;
}