#version 460 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 mvp;
out vec3 normalV;
//uniform mat4 mv;

void main()
{
  normalV = normal;
  gl_Position = mvp * vec4(vertexPos, 1.0);
}