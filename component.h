#pragma once
#include <transform.h>

class Scene;

class Component: public Transform
{
  public:
    Component(cy::Vec3f position): Transform(position), scene(nullptr) {};
    Component(): Transform(), scene(nullptr){};

    Scene* scene;

    virtual void start() = 0;
    virtual void beforeUpdate() = 0;
    virtual void aftherUpdate() = 0;
};