#version 460 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 4) in vec4 modelMatrixRow0;
layout (location = 5) in vec4 modelMatrixRow1;
layout (location = 6) in vec4 modelMatrixRow2;
layout (location = 7) in vec4 modelMatrixRow3;
layout (location = 8) in uint textureIdx;

out vec2 texCoord;

flat out uint textureIndex;

void main()
{
  mat4 modelMatrix = transpose(mat4(modelMatrixRow0, modelMatrixRow1, modelMatrixRow2, modelMatrixRow3));
  vec4 pos = modelMatrix *  vec4(vertexPos.xyz, 1.0);

  textureIndex = textureIdx;
  texCoord = uv;
  gl_Position = pos;
}