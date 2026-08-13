#pragma once
#include <../gameObject.h>

class ExampleObject: public GameObject
{
  public:
    using GameObject::GameObject;

    void start() override;
    void beforeUpdate() override;
    void afterUpdate() override;
  private:
};