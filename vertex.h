#pragma once
#include <cy/cyTriMesh.h>

struct Vertex 
{
  cy::Vec3f pos;
  cy::Vec3f normal;
  cy::Vec2f uv;
};