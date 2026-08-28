#pragma once
#define TMIN 0.01f
#define ERROR_MARGIN 0.01f

#include <hit.h>
#include <vertex.h>
#include <glm/glm.hpp>

class GameObject;

struct Ray 
{
  glm::vec3 origin;
  glm::vec3 direction;
  std::vector<Hit> hits;

  float maxDistance = INFINITY;
};

bool intersectBoundingVolume(Ray& r, glm::vec3* boundingVolume);
bool intersectTriangle(Ray& ray, GameObject* gameObject, Vertex &v0, Vertex &v1, Vertex &v2);