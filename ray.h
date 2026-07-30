#pragma once
#define TMIN 0.01f
#define ERROR_MARGIN 0.001f
#include <cy/cyTriMesh.h>
#include <hit.h>

struct Ray 
{
  cy::Vec3f origin;
  cy::Vec3f direction;
  std::vector<Hit> hits;
};