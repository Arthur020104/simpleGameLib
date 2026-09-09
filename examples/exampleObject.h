#pragma once
#include <../gameObject.h>

class ExampleObject: public GameObject
{
  public:
    using GameObject::GameObject;

    void start() override;
    void fixedUpdate() override;
    void beforeUpdate() override;
    void afterUpdate() override;
  private:
    bool rotating = false;
};