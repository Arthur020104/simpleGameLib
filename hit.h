#pragma once
#include <gameObject.h>
#include <cy/cyVector.h>
#include <vector>
#include <hit.h>

class GameObject;

struct Hit
{
  GameObject* hitObject;
  glm::vec3 point;
};
