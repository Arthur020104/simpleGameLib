#pragma once
#define TMIN 0.01f
#define ERROR_MARGIN 0.01f
#include <cy/cyTriMesh.h>
#include <hit.h>
#include <vertex.h>
class GameObject;

struct Ray 
{
  glm::vec3 origin;
  glm::vec3 direction;
  std::vector<Hit> hits;
};

bool intersectBoundingVolume(Ray& r, glm::vec3* boundingVolume);
bool intersectTriangle(Ray& ray, GameObject* gameObject, Vertex &v0, Vertex &v1, Vertex &v2);