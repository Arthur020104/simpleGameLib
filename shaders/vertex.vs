#version 460 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in uint materialIndice;

uniform mat4 mvp;
uniform mat4 modelMatrix;

out vec3 worldFragPos;
out vec3 normalV;
out vec2 texCoord;
flat out uint materialIdx;

void main()
{
  texCoord = uv;
  materialIdx = materialIndice;
  normalV = (modelMatrix * vec4(normal, 0.0)).xyz;

  worldFragPos = (modelMatrix * vec4(vertexPos, 1.0)).xyz;
  gl_Position = mvp * vec4(vertexPos, 1.0);
}