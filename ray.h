#pragma once
#define TMIN 0.01f
#define ERROR_MARGIN 0.01f
#include <cy/cyTriMesh.h>
#include <hit.h>
#include <vertex.h>
class GameObject;

struct Ray 
{
  cy::Vec3f origin;
  cy::Vec3f direction;
  std::vector<Hit> hits;
};

bool intersectBoundingVolume(Ray& r, cy::Vec3f* boundingVolume);
bool intersectTriangle(Ray& ray, GameObject* gameObject, Vertex &v0, Vertex &v1, Vertex &v2);