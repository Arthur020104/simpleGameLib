#version 460 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 invViewProjection;

out vec3 direction;

void main()
{
  vec4 pos = vec4(vertexPos.xyz, 1.0);

  direction = (invViewProjection * vec4(vertexPos, 1.0)).xyz;
  gl_Position = pos;
}