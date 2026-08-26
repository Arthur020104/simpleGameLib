#pragma once
#include <transform.h>

class Scene;

class Component: public Transform
{
  public:
    Component(glm::vec3 position): Transform(position), scene(nullptr) {};
    Component(): Transform(), scene(nullptr){};

    Scene* scene;

    virtual void start() = 0;
    virtual void beforeUpdate() = 0;
    virtual void afterUpdate() = 0;
};