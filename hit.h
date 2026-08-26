#pragma once
#include <hit.h>
#include <vector>
#include <gameObject.h>
#include <glm/glm.hpp>

class GameObject;

struct Hit
{
  GameObject* hitObject;
  glm::vec3 point;
};
