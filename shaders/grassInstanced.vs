#version 460 core
uint getMaterialID(uint idx);

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in uint materialIndice;
layout (location = 4) in vec4 modelMatrixRow0;
layout (location = 5) in vec4 modelMatrixRow1;
layout (location = 6) in vec4 modelMatrixRow2;
layout (location = 7) in vec4 modelMatrixRow3;

layout(std430, binding = 0) buffer MaterialBuffer {
  uint materialIDsPacked[];
};

uniform mat4 mv;
uniform uint verticesPerMesh;
uniform bool hasMaterialIndices;
uniform float baseY;
uniform vec3 wind;
uniform float grassStemSizeY;
uniform float time;

out vec3 worldFragPos;
out vec3 normalV;
out vec2 texCoord;

flat out uint materialIdx;
out float distanceFromBaseY;

void main()
{
  texCoord = uv;

  mat4 modelMatrix = transpose(mat4(modelMatrixRow0, modelMatrixRow1, modelMatrixRow2, modelMatrixRow3));

  normalV = (modelMatrix * vec4(normal, 0.0)).xyz;//non uniform scale breaks here, check if the performance cost will be acceptable to transpose(inverse(modelMatrix)) * vec4(normal, 0.0)).xyz;
  uint idx = uint(gl_InstanceID) * verticesPerMesh + uint(gl_VertexID);
  materialIdx = hasMaterialIndices ? getMaterialID(idx): materialIndice;
  
  vec4 worldPos = modelMatrix * vec4(vertexPos, 1.0);
  distanceFromBaseY = worldPos.y - baseY;

  float windFactor = distanceFromBaseY * 0.3 * sin(time + distanceFromBaseY * 0.1);
  
  worldFragPos = (worldPos  + vec4(wind * windFactor, 0.0)).xyz;
  worldPos += (worldPos  + vec4(wind * windFactor, 0.0));

  gl_Position = mv * worldPos;
}

uint getMaterialID(uint idx)
{
  uint word  = materialIDsPacked[idx / 4u];
  uint shift = (idx % 4u) * 8u;
  return (word >> shift) & 0xFFu;
}
