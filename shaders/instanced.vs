#version 460 core
uint getMaterialID(uint idx);

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 4) in vec4 modelMatrixRow0;
layout (location = 5) in vec4 modelMatrixRow1;
layout (location = 6) in vec4 modelMatrixRow2;
layout (location = 7) in vec4 modelMatrixRow3;

layout(std430, binding = 0) buffer MaterialBuffer {
  uint materialIDsPacked[];
};

uniform mat4 mv;
uniform uint verticesPerMesh;

out vec3 worldFragPos;
out vec3 normalV;
out vec2 texCoord;

flat out uint materialIdx;

void main()
{
  texCoord = uv;

  mat4 modelMatrix = transpose(mat4(modelMatrixRow0, modelMatrixRow1, modelMatrixRow2, modelMatrixRow3));

  normalV = (modelMatrix * vec4(normal, 0.0)).xyz;
  uint idx = uint(gl_InstanceID) * verticesPerMesh + uint(gl_VertexID);
  materialIdx = getMaterialID(idx);

  worldFragPos = (modelMatrix * vec4(vertexPos, 1.0)).xyz;
  gl_Position = mv * modelMatrix * vec4(vertexPos, 1.0);
}

uint getMaterialID(uint idx)
{
  uint word  = materialIDsPacked[idx / 4u];
  uint shift = (idx % 4u) * 8u;
  return (word >> shift) & 0xFFu;
}
